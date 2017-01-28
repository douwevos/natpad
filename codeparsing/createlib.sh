#You need to be root to run this script.
#
#Creates:
# /usr/local/include/gvlcodeparsing/ to which header files are copied
# /usr/local/lib/libgvlcodeparsing.a
# /usr/local/lib/libgvlcodeparsing-tm.a
# /usr/local/lib/libgvlcodeparsing-dm.a
# /usr/local/lib/libgvlcodeparsing-dtm.a

#Dependencies: gvlib

if [ "$(whoami)" != 'root' ]; then
  echo "You need to be root to run this script."
  exit 1
fi

if [ ! -d /usr/local/include/gvlib ]; then
  echo "Dependency missing: please create gvlib first."
  exit 1
fi


headerdir="/usr/local/include/gvlcodeparsing"

mkdir ___temp
cd src

echo "Compiling files for gvlcodeparsing..."
cc -c -O2 -o ../___temp/CharToken.o CharToken.c
cc -c -O2 -o ../___temp/CommentToken.o CommentToken.c
cc -c -O2 -o ../___temp/CTokeniser.o CTokeniser.c
cc -c -O2 -o ../___temp/elements.o elements.c
cc -c -O2 -o ../___temp/ElemInfo.o ElemInfo.c
cc -c -O2 -o ../___temp/ExtraToken.o ExtraToken.c
cc -c -O2 -o ../___temp/IncludeToken.o IncludeToken.c
cc -c -O2 -o ../___temp/keyWords.o keyWords.c
cc -c -O2 -o ../___temp/LastToken.o LastToken.c
cc -c -O2 -o ../___temp/LinenumberToken.o LinenumberToken.c
cc -c -O2 -o ../___temp/NumberToken.o NumberToken.c
cc -c -O2 -o ../___temp/PrimaryToken.o PrimaryToken.c
cc -c -O2 -o ../___temp/SecondaryToken.o SecondaryToken.c
cc -c -O2 -o ../___temp/StringElemInfoMap.o StringElemInfoMap.c
cc -c -O2 -o ../___temp/StringToken.o StringToken.c
cc -c -O2 -o ../___temp/Token.o Token.c
cc -c -O2 -o ../___temp/tokenFilter.o tokenFilter.c
cc -c -O2 -o ../___temp/Tokeniser.o Tokeniser.c
cc -c -O2 -o ../___temp/TokenList.o TokenList.c
cd ../___temp

echo "Creating libgvlcodeparsing.a"
ar rcs libgvlcodeparsing.a *.o
mv libgvlcodeparsing.a /usr/local/lib

rm CharToken.o
rm CommentToken.o
rm CTokeniser.o
rm ExtraToken.o
rm LinenumberToken.o
rm StringToken.o
cd ../src

echo "Compiling files for gvlcodeparsing-tm..."
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/CharToken.o CharToken.c
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/CommentToken.o CommentToken.c
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/CTokeniser.o CTokeniser.c
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/ExtraToken.o ExtraToken.c
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/LinenumberToken.o LinenumberToken.c
cc -c -O2 -DGVLCP_TEST_MODE -o ../___temp/StringToken.o StringToken.c
cd ../___temp

echo "Creating libgvlcodeparsing-tm.a"
ar rcs libgvlcodeparsing-tm.a *.o
mv libgvlcodeparsing-tm.a /usr/local/lib
rm -f *.o


cd ../src

echo "Compiling files for gvlcodeparsing-dm..."
cc -c -DDEBUG_MODE -o ../___temp/CharToken.o CharToken.c
cc -c -DDEBUG_MODE -o ../___temp/CommentToken.o CommentToken.c
cc -c -DDEBUG_MODE -o ../___temp/CTokeniser.o CTokeniser.c
cc -c -DDEBUG_MODE -o ../___temp/elements.o elements.c
cc -c -DDEBUG_MODE -o ../___temp/ElemInfo.o ElemInfo.c
cc -c -DDEBUG_MODE -o ../___temp/ExtraToken.o ExtraToken.c
cc -c -DDEBUG_MODE -o ../___temp/IncludeToken.o IncludeToken.c
cc -c -DDEBUG_MODE -o ../___temp/keyWords.o keyWords.c
cc -c -DDEBUG_MODE -o ../___temp/LastToken.o LastToken.c
cc -c -DDEBUG_MODE -o ../___temp/LinenumberToken.o LinenumberToken.c
cc -c -DDEBUG_MODE -o ../___temp/NumberToken.o NumberToken.c
cc -c -DDEBUG_MODE -o ../___temp/PrimaryToken.o PrimaryToken.c
cc -c -DDEBUG_MODE -o ../___temp/SecondaryToken.o SecondaryToken.c
cc -c -DDEBUG_MODE -o ../___temp/StringElemInfoMap.o StringElemInfoMap.c
cc -c -DDEBUG_MODE -o ../___temp/StringToken.o StringToken.c
cc -c -DDEBUG_MODE -o ../___temp/Token.o Token.c
cc -c -DDEBUG_MODE -o ../___temp/tokenFilter.o tokenFilter.c
cc -c -DDEBUG_MODE -o ../___temp/Tokeniser.o Tokeniser.c
cc -c -DDEBUG_MODE -o ../___temp/TokenList.o TokenList.c
cd ../___temp

echo "Creating libgvlcodeparsing-dm.a"
ar rcs libgvlcodeparsing-dm.a *.o
mv libgvlcodeparsing-dm.a /usr/local/lib

rm CharToken.o
rm CommentToken.o
rm CTokeniser.o
rm ExtraToken.o
rm LinenumberToken.o
rm StringToken.o
cd ../src

echo "Compiling files for gvlcodeparsing-dtm..."
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/CharToken.o CharToken.c
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/CommentToken.o CommentToken.c
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/CTokeniser.o CTokeniser.c
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/ExtraToken.o ExtraToken.c
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/LinenumberToken.o LinenumberToken.c
cc -c -DDEBUG_MODE -DGVLCP_TEST_MODE -o ../___temp/StringToken.o StringToken.c
cd ../___temp

echo "Creating libgvlcodeparsing-dtm.a"
ar rcs libgvlcodeparsing-dtm.a *.o
mv libgvlcodeparsing-dtm.a /usr/local/lib
cd ../include


if [ -d $headerdir ]; then
  echo "Removing old header files..."
  rm -rf /usr/local/include/gvlcodeparsing
fi

echo "Copying header files..."
cp -R gvlcodeparsing/ /usr/local/include

echo "Cleaning up..."
cd ..
rm -rf ___temp
find /usr/local/include/gvlcodeparsing -name .svn -type d -print0 | xargs -0 rm -rf
