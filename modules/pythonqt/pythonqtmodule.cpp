/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Link�ping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Erik Sund�n
 *
 **********************************************************************/

#include <modules/pythonqt/pythonqtmodule.h>

#include <modules/python/pythoninterface/pymodule.h>
#include "pythonqtmethods/pythonqtmethods.h"

namespace inviwo {

PythonQtModule::PythonQtModule() : InviwoModule() {
    setIdentifier("PythonQt");
    initPyQtModule();
}

PythonQtModule::~PythonQtModule(){
    delete inviwoPyQtModule_;
}

void PythonQtModule::initPyQtModule(){
    inviwoPyQtModule_ = new PyModule("inviwoqt");
    inviwoPyQtModule_->addMethod(new PyLoadNetwork());
}

} // namespace
