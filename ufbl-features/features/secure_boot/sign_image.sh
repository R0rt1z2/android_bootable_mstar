#!/bin/bash

# Copyright 2015-2021 Amazon.com, Inc. or its affiliates. All Rights Reserved.

# Required utilities: sha256sum, od, openssl, dd
set -e
set -o pipefail

function usage() {
	echo "Amazon FireOS common kernel signing tool"
	echo "Usage: sign_image.sh [-h] [-c <CA certificate>] [-k <private key>] [-f <config file>] "
	echo "                          [-m <additional config>] [-e <OpenSSL engine path>] "
	echo "                          <unsigned image> [signed image]"
	echo ""
	echo "  -h      Displays this help and exit"
	echo "  -c      Path to CA certificate file (PEM format)"
	echo "  -k      Path to CA private key"
	echo "  -f      Path to product-specific config file (for meta-data)"
	echo "  -m      Path to additional meta-data"
	echo "  -e      Path to custom OpenSSL engine"

        echo ""
	echo " If [signed image] is not provided, the default is <unsigned image>.signed"
}

# Detect OpenSSL version
set +e
openssl version | awk '$2 ~ /(^0\.)|(^1\.(0\.|1\.0))/ { exit 1 }'
OPENSSL_10=$?
set -e

ENGINE=

while getopts ":c:k:f:h:m:e:" o; do
	case "${o}" in
	c)
		CA=${OPTARG}
		;;
	k)
		PRIVATE_KEY=${OPTARG}
		;;
	f)
		CONFIG=${OPTARG}
		;;
	m)
		ADDITIONAL_CONFIG=${OPTARG}
		;;
	e)
		ENGINE=${OPTARG}
		;;
	*)
		usage
		exit -1
		;;
	esac
done
shift $((OPTIND-1))

IMAGE=$1
SIGNED_IMAGE=$2

if [[ -z $CA ]] || [[ -z $PRIVATE_KEY ]] || [[ -z $CONFIG ]] || [[ -z $IMAGE ]]; then
	usage
	exit -1
fi
if [[ -z $SIGNED_IMAGE ]]; then
	SIGNED_IMAGE=$IMAGE.signed
fi

# Compute the hash from image
IMAGE_HASH=`cat $IMAGE | sha256sum`
[[ $IMAGE_HASH =~ ^([0-9a-z]+) ]]
IMAGE_HASH=${BASH_REMATCH[1]}

# Get the page size of the image
PAGE_SIZE=`od -j 36 -t x4 -N 4 -An $IMAGE | sed -e 'y/abcdef/ABCDEF/'`
PAGE_SIZE=`echo "ibase=16; $PAGE_SIZE" | bc`

# Are we dealing with uImage header?
if [ `xxd -l 4 -ps $IMAGE` = '27051956' ]; then
	# Assume a 4k page size
	echo "uImage found, fixing page size to 4096 bytes..."
	PAGE_SIZE=4096
fi

echo "Generating CSR..."
# Generate a temp config file
cat $CONFIG $ADDITIONAL_CONFIG >> config.$$
openssl req -config config.$$ -new -newkey rsa:2048 -keyout userkey.pem.$$ -out usercert-req.pem.$$ -nodes

cat <<EOT > temp.conf.$$
[v3_req]
basicConstraints = CA:FALSE
keyUsage = nonRepudiation, digitalSignature
subjectAltName=URI:sha256://$IMAGE_HASH
EOT

echo "Signing CSR..."

if [[ $OPENSSL_10 -eq 1 ]]; then
	padding_scheme="-sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1"
else
	# OpenSSL 1.1 onwards uses digest rather than -1 to set the
	# salt length equal to digest length
	padding_scheme="-sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:digest"
fi

# Use custom engine?
if [ -z $ENGINE ]; then
	engine_args=
else
	engine_args="-CAkeyform engine -engine $ENGINE"
fi

openssl x509 -req -in usercert-req.pem.$$ \
		-CA $CA -CAkey $PRIVATE_KEY \
		-out usercert-out.der.$$ -outform DER -CAcreateserial -sha256 \
		-extfile temp.conf.$$ -extensions v3_req \
		$engine_args $padding_scheme

openssl x509 -in usercert-out.der.$$ -inform DER -noout -text -certopt no_validity,no_pubkey,no_sigdump

echo "Generating signed image..."
dd if=/dev/zero of=$IMAGE.sig.padded bs=$PAGE_SIZE count=1
dd if=usercert-out.der.$$ of=$IMAGE.sig.padded conv=notrunc
cat $IMAGE $IMAGE.sig.padded > $SIGNED_IMAGE
# Cleanup files
SRL="${CA%.*}".srl
rm -f $IMAGE.sig.padded *.$$ $SRL
exit 0
