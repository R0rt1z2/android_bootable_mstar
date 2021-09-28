#!/bin/bash
#
# Copyright 2019 Amazon.com, Inc. or its Affiliates. All rights reserved.
#
# Required utilities: sha256sum, od, openssl, head, tail
set -e
set -o pipefail

function usage() {
	echo "Amazon FireOS common kernel signature verification tool"
	echo "Usage: verify_image.sh [-h] [-c <CA certificate>] <signed image>"
	echo ""
	echo "  -h      Displays this help and exit"
	echo "  -c      Path to CA certificate file (PEM format)"
}

while getopts ":c:h:" o; do
	case "${o}" in
	c)
		CA=${OPTARG}
		;;
	*)
		usage
		exit
		;;
	esac
done
shift $((OPTIND-1))

IMAGE=$1

if [[ -z $CA ]] || [[ -z $IMAGE ]]; then
	usage
	exit -1
fi

# Gets the actual size of the image
IMAGE_SIZE=$(stat -c%s "$IMAGE")

# Get the page size of the image
PAGE_SIZE=`od -j 36 -t x4 -N 4 -An $IMAGE`
PAGE_SIZE=`echo "ibase=16; $PAGE_SIZE" | bc`

# Extract and verify the signature
CERT=`tail -c $PAGE_SIZE $IMAGE | openssl x509 -inform der`
OUTPUT=`echo "$CERT" | openssl verify -CAfile $CA 2>&1`

# Ignore "error 10" which means the certificate has expired,
# since bootloaders have no sense of time
#
# Any other errors are true errors
if [[ $OUTPUT =~ error ]] && [[ ! $OUTPUT =~ "error 10" ]]; then
	echo "Certificate failed authentication"
	exit -1
fi

# Extract the hash from signature
CERT_HASH=`echo "$CERT" | openssl x509 -noout -text | grep URI:sha256://`
[[ $CERT_HASH =~ URI:sha256://(.*)$ ]]
CERT_HASH=${BASH_REMATCH[1]}

# Compute the hash from image
IMAGE_HASH=`head -c -$PAGE_SIZE $IMAGE | sha256sum`
[[ $IMAGE_HASH =~ ^([0-9a-z]+) ]]
IMAGE_HASH=${BASH_REMATCH[1]}

if [[ "$IMAGE_HASH" != "$CERT_HASH" ]]; then
	echo "Mismatched hash between image and certificate"
	exit -1
else
	echo "Image signature is CORRECT"
fi
