/******************************************************************************
 *
 * Copyright (C) 1997-2020 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby
 * granted. No representations are made about the suitability of this software
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#ifndef MEMBERLIST_H
#define MEMBERLIST_H

#include <vector>
#include <algorithm>

#include "memberdef.h"
#include "linkedmap.h"
#include "types.h"
#include "membergroup.h"

class GroupDef;

int genericCompareMembers(const MemberDef *c1,const MemberDef *c2);

/** A list of MemberDef objects. */
class MemberList
{
    friend class MemberListIterator;
  public:
    MemberList();
    MemberList(MemberListType lt);
   ~MemberList();
    MemberListType listType() const { return m_listType; }
    static QCString listTypeAsString(MemberListType type);

    /* --- standard vector interface ---- */
    using Ptr = const MemberDef *;
    using Vec = std::vector<Ptr>;
    using iterator = typename Vec::iterator;
    using const_iterator = typename Vec::const_iterator;
    void push_back(const MemberDef *md)     { m_members.push_back(md); }
    iterator begin()                        { return m_members.begin();   }
    iterator end()                          { return m_members.end();     }
    const_iterator begin() const            { return m_members.cbegin();  }
    const_iterator end() const              { return m_members.cend();    }
    bool empty() const                      { return m_members.empty();   }
    size_t size() const                     { return m_members.size();    }
    Ptr& front()                            { return m_members.front();   }
    Ptr& back()                             { return m_members.back();    }

    /* --- other methods ---- */
    static bool lessThan(const MemberDef *md1,const MemberDef *md2)
    {
      return genericCompareMembers(md1,md2)<0;
    }
    void sort()
    {
      std::sort(m_members.begin(),m_members.end(),lessThan);
    }
    void inSort(const MemberDef *md)
    {
      m_members.insert( std::upper_bound( m_members.begin(), m_members.end(), md, lessThan), md);
    }
    void remove(const MemberDef *md)
    {
      auto it = std::find(m_members.begin(),m_members.end(),md);
      if (it!=m_members.end()) m_members.erase(it);
    }
    bool contains(const MemberDef *md)
    {
      auto it = std::find(m_members.begin(),m_members.end(),md);
      return it!=m_members.end();
    }

    int numDecMembers() const    { ASSERT(m_numDecMembers!=-1); return m_numDecMembers; }
    int numDecEnumValues() const { return m_numDecEnumValues; }
    int numDocMembers() const    { ASSERT(m_numDocMembers!=-1); return m_numDocMembers; }
    int numDocEnumValues() const { return m_numDocEnumValues; }
    bool needsSorting() const    { return m_needsSorting; }
    void countDecMembers();
    void countDocMembers();
    int countInheritableMembers(const ClassDef *inheritedFrom) const;
    void writePlainDeclarations(OutputList &ol,
               const ClassDef *cd,const NamespaceDef *nd,const FileDef *fd, const GroupDef *gd,
               const ClassDef *inheritedFrom,const char *inheritId) const;
    void writeDeclarations(OutputList &ol,
               const ClassDef *cd,const NamespaceDef *nd,const FileDef *fd,const GroupDef *gd,
               const char *title,const char *subtitle,
               bool showEnumValues=FALSE,bool showInline=FALSE,
               const ClassDef *inheritedFrom=0,MemberListType lt=MemberListType_pubMethods) const;
    void writeDocumentation(OutputList &ol,const char *scopeName,
               const Definition *container,const char *title,
               bool showEnumValues=FALSE,bool showInline=FALSE) const;
    void writeSimpleDocumentation(OutputList &ol,const Definition *container) const;
    void writeDocumentationPage(OutputList &ol,
               const char *scopeName, const DefinitionMutable *container) const;
    void writeTagFile(std::ostream &);
    bool declVisible() const;
    void addMemberGroup(MemberGroup *mg);
    void setInGroup(bool inGroup) { m_inGroup=inGroup; }
    void setInFile(bool inFile) { m_inFile=inFile; }
    void addListReferences(Definition *def);
    void findSectionsInDocumentation(const Definition *d);
    void setNeedsSorting(bool b);
    const MemberGroupRefList &getMemberGroupList() const { return m_memberGroupRefList; }
    void setAnonymousEnumType();
    void setAnchors();

  private:
    int countEnumValues(const MemberDef *md) const;
    int m_numDecMembers; // number of members in the brief part of the memberlist
    int m_numDecEnumValues;
    int m_numDocMembers; // number of members in the detailed part of the memberlist
    int m_numDocEnumValues;
    MemberGroupRefList m_memberGroupRefList;
    bool m_inGroup; // is this list part of a group definition
    bool m_inFile;  // is this list part of a file definition
    MemberListType m_listType;
    bool m_needsSorting;
    Vec m_members;
};

class MemberLinkedRefMap : public LinkedRefMap<const MemberDef>
{
};

class MemberLists : public std::vector< std::unique_ptr<MemberList> >
{
  public:
    MemberLists() = default;
    const std::unique_ptr<MemberList> &get(MemberListType lt)
    {
      // find the list with the given type
      auto it = std::find_if(begin(),end(),[&lt](const auto &ml) { return ml->listType()==lt; });
      if (it!=end()) return *it;
      // or create a new list if it is not found
      emplace_back(std::make_unique<MemberList>(lt));
      return back();
    }

  private:
    MemberLists(const MemberLists &) = delete;
    MemberLists &operator=(const MemberLists &) = delete;
};


#endif
