enum levelEnum  {Ground, Platform, Vinebox, SmallTree, TreeTrunk, TreeTop, Fence1, Fence2, Cattail,
HouseSide, HouseBack, HouseRoof, SandBox, FencePart1, FencePart2, lvl2Ground, testbarn,
barnback, barnfrontside, barnfronttop, barnroof, barnside, barnfrontside2,roundbail, squarebail, woodpile, nocoll = 99};



enum enemy{simpleEnemy, tractor};


enum object{orange, branch, gateone, gatetwo};

//0 = level, 1 = enemy = 2 = tripping/stumbling object, 3 = item to gather , 99 = player
enum Collisiontype {ctLevel, ctEnemy, ctStumble, ctCollect, ctUnkillable, ctNothing };

