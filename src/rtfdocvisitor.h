/******************************************************************************
 *
 *
 *
 *
 * Copyright (C) 1997-2015 by Dimitri van Heesch.
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

#ifndef RTFDOCVISITOR_H
#define RTFDOCVISITOR_H

#include <iostream>

#include "docvisitor.h"
#include "qcstring.h"

class CodeOutputInterface;
class TextStream;

/*! @brief Concrete visitor implementation for RTF output. */
class RTFDocVisitor : public DocVisitor
{
  public:
    RTFDocVisitor(TextStream &t,CodeOutputInterface &ci,const QCString &langExt);

    //--------------------------------------
    // visitor functions for leaf nodes
    //--------------------------------------

    void visit(DocWord *);
    void visit(DocLinkedWord *);
    void visit(DocWhiteSpace *);
    void visit(DocSymbol *);
    void visit(DocEmoji *);
    void visit(DocURL *);
    void visit(DocLineBreak *);
    void visit(DocHorRuler *);
    void visit(DocStyleChange *);
    void visit(DocVerbatim *);
    void visit(DocAnchor *);
    void visit(DocInclude *);
    void visit(DocIncOperator *);
    void visit(DocFormula *);
    void visit(DocIndexEntry *);
    void visit(DocSimpleSectSep *);
    void visit(DocCite *);

    //--------------------------------------
    // visitor functions for compound nodes
    //--------------------------------------

    void visitPre(DocAutoList *);
    void visitPost(DocAutoList *);
    void visitPre(DocAutoListItem *);
    void visitPost(DocAutoListItem *);
    void visitPre(DocPara *);
    void visitPost(DocPara *);
    void visitPre(DocRoot *);
    void visitPost(DocRoot *);
    void visitPre(DocSimpleSect *);
    void visitPost(DocSimpleSect *);
    void visitPre(DocTitle *);
    void visitPost(DocTitle *);
    void visitPre(DocSimpleList *);
    void visitPost(DocSimpleList *);
    void visitPre(DocSimpleListItem *);
    void visitPost(DocSimpleListItem *);
    void visitPre(DocSection *s);
    void visitPost(DocSection *);
    void visitPre(DocHtmlList *s);
    void visitPost(DocHtmlList *s);
    void visitPre(DocHtmlListItem *);
    void visitPost(DocHtmlListItem *);
    //void visitPre(DocHtmlPre *);
    //void visitPost(DocHtmlPre *);
    void visitPre(DocHtmlDescList *);
    void visitPost(DocHtmlDescList *);
    void visitPre(DocHtmlDescTitle *);
    void visitPost(DocHtmlDescTitle *);
    void visitPre(DocHtmlDescData *);
    void visitPost(DocHtmlDescData *);
    void visitPre(DocHtmlTable *t);
    void visitPost(DocHtmlTable *t);
    void visitPre(DocHtmlCaption *);
    void visitPost(DocHtmlCaption *);
    void visitPre(DocHtmlRow *);
    void visitPost(DocHtmlRow *) ;
    void visitPre(DocHtmlCell *);
    void visitPost(DocHtmlCell *);
    void visitPre(DocInternal *);
    void visitPost(DocInternal *);
    void visitPre(DocHRef *);
    void visitPost(DocHRef *);
    void visitPre(DocHtmlHeader *);
    void visitPost(DocHtmlHeader *) ;
    void visitPre(DocImage *);
    void visitPost(DocImage *);
    void visitPre(DocDotFile *);
    void visitPost(DocDotFile *);
    void visitPre(DocMscFile *);
    void visitPost(DocMscFile *);
    void visitPre(DocDiaFile *);
    void visitPost(DocDiaFile *);
    void visitPre(DocLink *);
    void visitPost(DocLink *);
    void visitPre(DocRef *ref);
    void visitPost(DocRef *);
    void visitPre(DocSecRefItem *);
    void visitPost(DocSecRefItem *);
    void visitPre(DocSecRefList *);
    void visitPost(DocSecRefList *);
    void visitPre(DocParamSect *);
    void visitPost(DocParamSect *);
    void visitPre(DocParamList *);
    void visitPost(DocParamList *);
    void visitPre(DocXRefItem *);
    void visitPost(DocXRefItem *);
    void visitPre(DocInternalRef *);
    void visitPost(DocInternalRef *);
    void visitPre(DocText *);
    void visitPost(DocText *);
    void visitPre(DocHtmlBlockQuote *);
    void visitPost(DocHtmlBlockQuote *);
    void visitPre(DocVhdlFlow *);
    void visitPost(DocVhdlFlow *);
    void visitPre(DocParBlock *);
    void visitPost(DocParBlock *);

  private:

    //--------------------------------------
    // helper functions
    //--------------------------------------

    void filter(const QCString &str,bool verbatim=FALSE);
    void startLink(const QCString &ref,const QCString &file,
                   const QCString &anchor);
    void endLink(const QCString &ref);
    QCString getStyle(const QCString &name);

    int indentLevel() const;
    void incIndentLevel();
    void decIndentLevel();

    void includePicturePreRTF(const QCString &name, bool isTypeRTF, bool hasCaption, bool inlineImage = FALSE);
    void includePicturePostRTF(bool isTypeRTF, bool hasCaption, bool inlineImage = FALSE);
    void writeDotFile(const QCString &fileName, bool hasCaption,const QCString &srcFile,int srcLine);
    void writeDotFile(DocDotFile *);
    void writeMscFile(const QCString &fileName, bool hasCaption,const QCString &srcFile,int srcLine);
    void writeMscFile(DocMscFile *);
    void writeDiaFile(DocDiaFile *);
    void writePlantUMLFile(const QCString &fileName, bool hasCaption);

    //--------------------------------------
    // state variables
    //--------------------------------------

    TextStream &m_t;
    CodeOutputInterface &m_ci;
    bool m_insidePre = false;
    bool m_hide = false;
    bool m_lastIsPara = false;
    QCString m_langExt;

    static const int maxIndentLevels = 13;
    int m_indentLevel = 0;
    struct RTFListItemInfo
    {
      bool isEnum;
      int number;
      char type;
    };
    RTFListItemInfo m_listItemInfo[maxIndentLevels];
};

#endif
