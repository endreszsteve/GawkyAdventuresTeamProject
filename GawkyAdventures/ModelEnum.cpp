enum levelEnum  {Ground, Platform, Vinebox, SmallTree, TreeTrunk, TreeTop, Fence1, Fence2, Cattail,
HouseSide, HouseBack, HouseRoof, SandBox};



enum enemy{simpleEnemy};


enum object{orange, branch};

//0 = level, 1 = enemy = 2 = tripping/stumbling object, 3 = item to gather , 99 = player
enum Collisiontype {ctLevel, ctEnemy, ctStumble, ctCollect };

