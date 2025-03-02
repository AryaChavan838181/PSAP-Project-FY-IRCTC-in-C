#!/bin/bash

echo "🔄 Compiling..."
make clean
make

if [ $? -eq 0 ]; then
    echo "✅ Compilation successful!"
    echo "🚀 Running the application..."
    ./main
else
    echo "❌ Compilation failed!"
fi
