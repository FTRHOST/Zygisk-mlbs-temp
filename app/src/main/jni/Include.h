#pragma once

#include <string>
#include "Utils.h"
#include "jniStuff.h"
#include "ByNameModding/Tools.h"
#include "fake_dlfcn.h"
#include "Il2Cpp.h"
#include "include/Utils/MonoString.h"
#include "obfuscate.h"
#include "Vector3.h"

using namespace std;

typedef monoString MonoString;

int glWidth, glHeight;

/*
const-string v0, "MEOW"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
*/
