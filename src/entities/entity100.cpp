/*
 * file: entity100.cpp
 *
 * Copyright 2015, Dr. Cirilo Bernardo (cirilo.bernardo@gmail.com)
 *
 * Description: IGES Entity 100: Circle, Section 4.3, p.66+ (94+)
 *
 * This file is part of libIGES.
 *
 * libIGES is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libIGES is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libIGES.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <sstream>
#include <error_macros.h>
#include <iges.h>
#include <iges_io.h>
#include <entity100.h>

using namespace std;

IGES_ENTITY_100::IGES_ENTITY_100( IGES* aParent ) : IGES_ENTITY( aParent )
{
    entityType = 100;
    form = 0;

    zOffset = 0.0;
    xCenter = 0.0;
    yCenter = 0.0;
    xStart = 1.0;
    yStart = 0.0;
    xEnd = 1.0;
    yEnd = 0.0;

    return;
}   // IGES_ENTITY_100( IGES* aParent )


IGES_ENTITY_100::~IGES_ENTITY_100()
{
    return;
}   // ~IGES_ENTITY_100()


bool IGES_ENTITY_100::associate( std::vector<IGES_ENTITY*>* entities )
{
    if( !IGES_ENTITY::associate( entities ) )
    {
        ERRMSG << "\n + [INFO] failed to establish associations\n";
        return false;
    }

    return true;
}


bool IGES_ENTITY_100::format( int &index )
{
    pdout.clear();

    if( index < 1 || index > 9999999 )
    {
        ERRMSG << "\n + [INFO] invalid Parameter Data Sequence Number\n";
        return false;
    }

    parameterData = index;

    if( !parent )
    {
        ERRMSG << "\n + [INFO] method invoked with no parent IGES object\n";
        return false;
    }

    char pd = parent->globalData.pdelim;
    char rd = parent->globalData.rdelim;
    double uir = parent->globalData.minResolution;

    ostringstream ostr;
    ostr << entityType << pd;
    string fStr = ostr.str();
    string tStr;

    if( !FormatPDREal( tStr, zOffset, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format zOffset\n";
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    if( !FormatPDREal( tStr, xCenter, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format xCenter\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    if( !FormatPDREal( tStr, yCenter, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format yCenter\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    if( !FormatPDREal( tStr, xStart, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format xStart\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    if( !FormatPDREal( tStr, yStart, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format yStart\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    if( !FormatPDREal( tStr, xEnd, pd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format xEnd\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    // XXX - TO BE IMPLEMENTED
    // NOTE: 2 sets of OPTIONAL parameters may exist at the end of
    // any PD; see p.32/60+ for details; if optional parameters
    // need to be written then we should use 'pd' rather than 'rd'
    // in this call to FormatPDREal()
    if( !FormatPDREal( tStr, yEnd, rd, uir ) )
    {
        ERRMSG << "\n + [INFO] could not format yEnd\n";
        pdout.clear();
        return false;
    }

    AddPDItem( tStr, fStr, pdout, index, sequenceNumber, pd, rd );

    paramLineCount = index - parameterData;
    return true;
}


bool IGES_ENTITY_100::Unlink( IGES_ENTITY* aChild )
{
    return IGES_ENTITY::Unlink( aChild );
}


bool IGES_ENTITY_100::IsOrphaned( void )
{
    // TRUE when there are no parent references regardless of dependency setting
    // XXX - DEBUG ONLY
    return false;

    if( refs.empty() )
        return true;

    return false;
}


bool IGES_ENTITY_100::IGES_ENTITY_100::AddReference( IGES_ENTITY* aParentEntity )
{
    return IGES_ENTITY::AddReference( aParentEntity );
}


bool IGES_ENTITY_100::DelReference( IGES_ENTITY* aParentEntity )
{
    return IGES_ENTITY::DelReference( aParentEntity );
}


bool IGES_ENTITY_100::ReadDE( IGES_RECORD* aRecord, std::ifstream& aFile, int& aSequenceVar )
{
    if( !IGES_ENTITY::ReadDE( aRecord, aFile, aSequenceVar ) )
    {
        ERRMSG << "\n + [INFO] failed to read Directory Entry\n";
        return false;
    }

    structure = 0;                  // N.A.
    hierarchy = STAT_HIER_ALL_SUB;  // field ignored

    if( form != 0 )
    {
        ERRMSG << "\n + [CORRUPT FILE] non-zero Form Number in Circle\n";
        cerr << " + DE: " << aRecord->index << "\n";
        return false;
    }

    return true;
}


bool IGES_ENTITY_100::ReadPD( std::ifstream& aFile, int& aSequenceVar )
{
    if( !IGES_ENTITY::ReadPD( aFile, aSequenceVar ) )
    {
        ERRMSG << "\n + [INFO] could not read data for Circle Entity\n";
        return false;
    }

    int idx = 0;
    bool eor = false;
    char pd = parent->globalData.pdelim;
    char rd = parent->globalData.rdelim;

    if( !ParseReal( pdout, idx, zOffset, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no zOffset datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, xCenter, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no xCenter datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, yCenter, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no yCenter datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, xStart, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no xStart datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, yStart, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no yStart datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, xEnd, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no xEnd datum for Circle Entity\n";
        return false;
    }

    if( !ParseReal( pdout, idx, yEnd, eor, pd, rd ) )
    {
        ERRMSG << "\n + [BAD FILE] no yEnd datum for Circle Entity\n";
        return false;
    }

    if( eor )
    {
        pdout.clear();
        return true;
    }

    // XXX - TO BE IMPLEMENTED
    // if( eor )
    // {
    //     ReadComments();
    //     pdout.clear();
    //     return true;
    // }

    // ReadExtraParams()
    // ReadComments();
    // pdout.clear();
    // return true;

    return false;
}


bool IGES_ENTITY_100::SetEntityForm( int aForm )
{
    if( aForm == 0 )
        return true;

    ERRMSG << "\n + [BUG] Circle Entity only supports Form 0 (requested form: ";
    cerr << aForm << ")\n";
    return false;
}


bool IGES_ENTITY_100::SetDependency( IGES_STAT_DEPENDS aDependency )
{
    // XXX - TO BE IMPLEMENTED
    ERRMSG << "\n + [WARNING] TO BE IMPLEMENTED\n";
    return false;
}


bool IGES_ENTITY_100::SetEntityUse( IGES_STAT_USE aUseCase )
{
    // XXX - TO BE IMPLEMENTED
    ERRMSG << "\n + [WARNING] TO BE IMPLEMENTED\n";
    return false;
}


bool IGES_ENTITY_100::SetHierarchy( IGES_STAT_HIER aHierarchy )
{
    // XXX - TO BE IMPLEMENTED
    ERRMSG << "\n + [WARNING] TO BE IMPLEMENTED\n";
    return false;
}
