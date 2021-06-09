#!/usr/bin/env python3
print ( '''router X at t=0
 	Y	Z
 Y	2	INF
 Z	INF	7
 
 router Y at t=0
 	X	Z
 X	2	INF
 Z	INF	1
 
 router Z at t=0
 	X	Y
 X	7	INF
 Y	INF	1
 
 router X at t=1
 	Y	Z
 Y	2	8
 Z	3	7
 
 router Y at t=1
 	X	Z
 X	2	8
 Z	9	1
 
 router Z at t=1
 	X	Y
 X	7	3
 Y	9	1
 
 router X at t=2
 	Y	Z
 Y	2	8
 Z	3	7
 
 router Y at t=2
 	X	Z
 X	2	4
 Z	5	1
 
 router Z at t=2
 	X	Y
 X	7	3
 Y	9	1
 
 router X: Y is 2 routing through Y
 router X: Z is 3 routing through Y
 router Y: X is 2 routing through X
 router Y: Z is 1 routing through Z
 router Z: X is 3 routing through Y
 router Z: Y is 1 routing through Y''')