#!/bin/bash

cd ../thorin
git cherry-pick --no-commit origin/rv_module_build
cd ../build

exec ninja
