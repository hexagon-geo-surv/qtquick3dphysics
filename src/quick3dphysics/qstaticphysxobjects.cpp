// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qstaticphysxobjects_p.h"

QT_BEGIN_NAMESPACE

StaticPhysXObjects s_physx = StaticPhysXObjects();

StaticPhysXObjects &StaticPhysXObjects::getReference()
{
    return s_physx;
}

QT_END_NAMESPACE
