#You need to be root to run this script.
#
#Creates:
# /usr/local/include/gvlib/ to which header files are copied
# /usr/local/lib/libgvlib.a
# /usr/local/lib/libgvlib-dm.a


if [ "$(whoami)" != 'root' ]; then
  echo "You need to be root to run this script."
  exit 1
fi


headerdir="/usr/local/include/gvlib"

mkdir ___temp

echo "Compiling files for gvlib..."
cd auxiliaries/
cc -c -O2 -o ../___temp/Reference.o Reference.c
cc -c -O2 -o ../___temp/RefHashList.o RefHashList.c
cc -c -O2 -o ../___temp/RefList.o RefList.c
cd ../src/
cc -c -O2 -o ../___temp/array.o array.c
cc -c -O2 -o ../___temp/byteOrder.o byteOrder.c
cc -c -O2 -o ../___temp/char.o char.c
cc -c -O2 -o ../___temp/errors.o errors.c
cc -c -O2 -o ../___temp/FileInputStream.o FileInputStream.c
cc -c -O2 -o ../___temp/FileOutputStream.o FileOutputStream.c
cc -c -O2 -o ../___temp/FileReader.o FileReader.c
cc -c -O2 -o ../___temp/FileWriter.o FileWriter.c
cc -c -O2 -o ../___temp/hashCode.o hashCode.c
cc -c -O2 -o ../___temp/InputStream.o InputStream.c
cc -c -O2 -o ../___temp/List.o List.c
cc -c -O2 -o ../___temp/Map.o Map.c
cc -c -O2 -o ../___temp/mem.o mem.c
cc -c -O2 -o ../___temp/Object.o Object.c
cc -c -O2 -o ../___temp/OutputStream.o OutputStream.c
cc -c -O2 -o ../___temp/PushbackStream.o PushbackStream.c
cc -c -O2 -o ../___temp/Reader.o Reader.c
cc -c -O2 -o ../___temp/Set.o Set.c
cc -c -O2 -o ../___temp/String.o String.c
cc -c -O2 -o ../___temp/StringBuilder.o StringBuilder.c
cc -c -O2 -o ../___temp/TypeData.o TypeData.c
cc -c -O2 -o ../___temp/Utf8InputStream.o Utf8InputStream.c
cc -c -O2 -o ../___temp/Utf8OutputStream.o Utf8OutputStream.c
cc -c -O2 -o ../___temp/utils.o utils.c
cc -c -O2 -o ../___temp/Writer.o Writer.c
cd ../___temp

echo "Creating libgvlib.a"
ar rcs libgvlib.a *.o
mv libgvlib.a /usr/local/lib
rm -f *.o

echo "Compiling files for gvlib-dm..."
cd ../auxiliaries/
cc -c -D DEBUG_MODE -o ../___temp/Reference.o Reference.c
cc -c -D DEBUG_MODE -o ../___temp/RefHashList.o RefHashList.c
cc -c -D DEBUG_MODE -o ../___temp/RefList.o RefList.c
cd ../src/
cc -c -D DEBUG_MODE -o ../___temp/array.o array.c
cc -c -D DEBUG_MODE -o ../___temp/byteOrder.o byteOrder.c
cc -c -D DEBUG_MODE -o ../___temp/char.o char.c
cc -c -D DEBUG_MODE -o ../___temp/errors.o errors.c
cc -c -D DEBUG_MODE -o ../___temp/FileInputStream.o FileInputStream.c
cc -c -D DEBUG_MODE -o ../___temp/FileOutputStream.o FileOutputStream.c
cc -c -D DEBUG_MODE -o ../___temp/FileReader.o FileReader.c
cc -c -D DEBUG_MODE -o ../___temp/FileWriter.o FileWriter.c
cc -c -D DEBUG_MODE -o ../___temp/hashCode.o hashCode.c
cc -c -D DEBUG_MODE -o ../___temp/InputStream.o InputStream.c
cc -c -D DEBUG_MODE -o ../___temp/List.o List.c
cc -c -D DEBUG_MODE -o ../___temp/Map.o Map.c
cc -c -D DEBUG_MODE -o ../___temp/mem.o mem.c
cc -c -D DEBUG_MODE -o ../___temp/Object.o Object.c
cc -c -D DEBUG_MODE -o ../___temp/OutputStream.o OutputStream.c
cc -c -D DEBUG_MODE -o ../___temp/PushbackStream.o PushbackStream.c
cc -c -D DEBUG_MODE -o ../___temp/Reader.o Reader.c
cc -c -D DEBUG_MODE -o ../___temp/Set.o Set.c
cc -c -D DEBUG_MODE -o ../___temp/String.o String.c
cc -c -D DEBUG_MODE -o ../___temp/StringBuilder.o StringBuilder.c
cc -c -D DEBUG_MODE -o ../___temp/TypeData.o TypeData.c
cc -c -D DEBUG_MODE -o ../___temp/Utf8InputStream.o Utf8InputStream.c
cc -c -D DEBUG_MODE -o ../___temp/Utf8OutputStream.o Utf8OutputStream.c
cc -c -D DEBUG_MODE -o ../___temp/utils.o utils.c
cc -c -D DEBUG_MODE -o ../___temp/Writer.o Writer.c
cd ../___temp

echo "Creating libgvlib-dm.a"
ar rcs libgvlib-dm.a *.o
mv libgvlib-dm.a /usr/local/lib
cd ../include

if [ -d $headerdir ]; then
  echo "Removing old header files..."
  rm -rf /usr/local/include/gvlib
fi

echo "Copying header files..."
cp -R gvlib/ /usr/local/include

echo "Cleaning up..."
cd ..
rm -rf ___temp
find /usr/local/include/gvlib -name .svn -type d -print0 | xargs -0 rm -rf
