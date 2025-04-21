


default:
	mkdir -p bin
	g++ src/*.cpp -o bin/bns.out

clean:
	rm -rf bin
