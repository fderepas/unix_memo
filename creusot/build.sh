#!/bin/bash
# Build a multipass image with a Zephyr development environement
set -e
set -x

cd `dirname $0`
IMAGE_NAME=rtos-creusot

# install multipass if needed
if [[ `which multipass | wc -l` == 0 ]]; then
    echo '*** install multipass'
    sudo snap install multipass
fi

# delete former image if needed
if [[ `multipass list |grep $IMAGE_NAME| wc -l` == 1 ]]; then
    multipass stop --force $IMAGE_NAME
    multipass delete $IMAGE_NAME
    multipass purge
fi

# setup image
multipass launch --name $IMAGE_NAME
#multipass transfer net_and_west.sh $IMAGE_NAME:/home/ubuntu/net_and_west.sh
multipass stop $IMAGE_NAME
multipass set local.$IMAGE_NAME.disk=30G
multipass mount $PWD $IMAGE_NAME:/home/ubuntu/script

# install needed packages
multipass exec $IMAGE_NAME -- sudo bash /home/ubuntu/script/script_root.sh
#multipass exec $IMAGE_NAME -- bash /home/ubuntu/script/script_user.sh
#multipass restart $IMAGE_NAME

# take a snapshot named 'initial'
#multipass stop $IMAGE_NAME
#multipass snapshot $IMAGE_NAME --name initial

