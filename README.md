# LIC

Line Integral Convolution for 2d vector fields

This is an implementation of a visualization technique for 2d vector
fields that I used during my graduate studies (yeah, I'm a 
mathematician!). This means it's a pretty old technique. Here's the
reference used for this implementation:

Cabral, Brian; Leedom, Leith Casey (August 2–6, 1993). "Imaging 
Vector Fields Using Line Integral Convolution". Proceedings of the 
20th annual conference on Computer graphics and interactive 
techniques. SIGGRAPH '93. Anaheim, California. pp. 263–270.

The general idea is to calculate the line integral of a random noise 
map along a vector field, and to color the starting point based on
that integral. You end up with smooth colors along the vector field
and discontinuities perpendicular to it.

Not sure if this is still en vogue today, probably there are 
better methods now. 

It's fun to play around with it.
