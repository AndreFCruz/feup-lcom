cd src
make clean install
mv planetary_defense ../
cd ..
strip --strip-all planetary_defense
