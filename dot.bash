for file in *.dot; do
    dot -Tpng "$file" -o "${file%.dot}.png"
done
