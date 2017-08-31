#!/usr/bin/env bash
cd $(dirname $0)
twistd -n web --path public
