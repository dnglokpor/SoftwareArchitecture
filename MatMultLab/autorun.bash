echo "Tiled runs with tiles sizes between 1 and 25" > tiledReport.txt
for i in {1..25}
do
	echo "tilesize = $i" >> tiledReport.txt
	(time ./MatMultTiledX $i) &>> tiledReport.txt
done
