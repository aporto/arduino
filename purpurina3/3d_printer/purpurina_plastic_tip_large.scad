
difference() {
    round_cube(39, 15, 55, 2);
    
    translate ([1.5, 1.5, 3]) 
    round_cube(36, 12, 200, 1);
    
    translate ([11, 5, 20]) 
    rotate([90,0,0])
    
    round_cube(18, 250, 100, 2);
    
    translate ([20, 15, 35])   
    cube([18,4,60], true);    
}  

difference() {
    translate ([20, 10, 0]) 
    cylinder (h=55, r1=12, r2=12);
    
    translate ([20, 10, 2]) 
    cylinder (h=80, r1=10, r2=10);

    translate ([10, 0, -20]) 
    cube([60,30,200], true);
}

   
module round_cube(l=40,w=30,h=20,r=5,$fn=30){
	hull(){ 
		translate ([r, r, 0]) cylinder (h = h, r=r);
		translate ([r, w-r, 0]) cylinder (h = h, r=r);
		translate ([l-r,w-r, 0]) cylinder (h = h, r=r);
		translate ([l-r, r, 0]) cylinder (h = h, r=r);
	}
}
