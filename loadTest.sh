echo "loadtest is running" >> log.txt

j=0
for i in {1..50000000}
do
   j=$((j + i))
done

