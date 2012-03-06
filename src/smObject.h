//
//  smObject.h
//  spatialMediaTest
//
//  Created by Jared Schiffman on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef spatialMediaTest_smObject_h
#define spatialMediaTest_smObject_h
#include <string>



class smObject
{
    
public:
    
    smObject( std::string _NAME, int _AREA);
        
    int             area;
    std::string     name;
    int             blobID;
    
};


#endif
