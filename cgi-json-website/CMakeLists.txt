cmake_minimum_required (VERSION 2.6)
project (HELLOWORLD)
add_executable(HELLOWORLD cgi-helloworld.c)


# sudo mv HELLOWORLD /usr/lib/cgi-bin/HELLOWORLD
# sudo chmod a+rx /usr/lib/cgi-bin/HELLOWORLD