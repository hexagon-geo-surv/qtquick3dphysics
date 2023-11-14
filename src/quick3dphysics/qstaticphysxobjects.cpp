// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial

#include "qstaticphysxobjects_p.h"
#include <QGlobalStatic>
#include <new>

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(StaticPhysXObjects, s_physx);

StaticPhysXObjects &StaticPhysXObjects::getReference()
{
    return *s_physx;
}

QT_END_NAMESPACE
