

//translate([0,0,55])
//cylinder(h = 55, r1 = 26, r2 = 26, center = true);

//translate([0,0,0])
//cube([24,14,30],true);

difference() {
    block();    
}

//translate([0,0,10])
//cylinder(h = 2, r1=15, r2=15, center=true);


module fill_gaps() 
{
    difference() {
        translate([0,0, -7.5])
        cylinder(h = 14, r1=36, r2=36);
        
        translate([0,11,1])
        cube([160,60,22], true);
        
        translate([29,0,1])
        cube([20,100,20], true);

        translate([-29,0,1])
        cube([20,100,20], true);
    }
}


module gluestick() {
    translate([0,0,2])
    cylinder(h = 100, r1 = 16, r2 = 16);
    //translate([0,0, 32])
    //cylinder(h = 32, r1 = 28, r2 = 28);
}

module vertical_stick()
{
    translate([6,0,0])
    {
        difference() {
        // retangulo vertical do suporte
        translate([-8.5,-30,0])
        cube([17,20,60]);

        //cilindro negativo para prender no mastro
        translate([0,-36,-20])
        cylinder(h = 102.5, r1 = 16, r2 = 16);
        
        // buraco para tie-wrap
        translate([10, -18, 45])
        cube ([50,2,5], true);

        // buraco para tie-wrap
        translate([10, -18, 32])
        cube ([50,2,5], true);
            
        // buraco para tie-wrap
        translate([10, -18, 20])
        cube ([50,2,5], true);
            
        gluestick();
    }
}
}

module block() {
    difference() {
        union() {
            difference () {
                // base ao redor do servo
                translate([0,-6,0])
                cube([43,22,5], true);
        
                // servo
                translate([0,0,0])
                cube([24,14,100],true);
            }                 
        }            
    }
    
    vertical_stick();
    
}

