
difference() {
    round_cube(39, 15, 20, 2);
    
    translate ([1.5, 1.5, 3]) 
    round_cube(36, 12, 20, 1);
    
    translate ([11, 5, 5]) 
    rotate([90,0,0])
    
    round_cube(18, 25, 10, 2);
}        

module round_cube(l=40,w=30,h=20,r=5,$fn=30){
	hull(){ 
		translate ([r, r, 0]) cylinder (h = h, r=r);
		translate ([r, w-r, 0]) cylinder (h = h, r=r);
		translate ([l-r,w-r, 0]) cylinder (h = h, r=r);
		translate ([l-r, r, 0]) cylinder (h = h, r=r);
	}
}
