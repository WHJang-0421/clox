cd build
cmake ..
cmake --build .
if [ "$#" -eq 0 ]
then
    ./main.o
else
    ./main.o ../$1
fi
