target:
	g++ -ggdb3 -c src/*.cpp
	mv *.o obj/
	g++ -ggdb3 -o tile-sim obj/*

clean:
	rm obj/*

