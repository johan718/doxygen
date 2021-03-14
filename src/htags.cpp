/******************************************************************************
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

#include <stdio.h>

#include <unordered_map>
#include <string>

#include "htags.h"
#include "util.h"
#include "message.h"
#include "config.h"
#include "portable.h"
#include "fileinfo.h"

bool Htags::useHtags = FALSE;

static Dir g_inputDir;
static std::unordered_map<std::string,std::string> g_symbolMap;

/*! constructs command line of htags(1) and executes it.
 *  \retval TRUE success
 *  \retval FALSE an error has occurred.
 */
bool Htags::execute(const QCString &htmldir)
{
  const StringVector &inputSource = Config_getList(INPUT);
  bool quiet = Config_getBool(QUIET);
  bool warnings = Config_getBool(WARNINGS);
  QCString htagsOptions = ""; //Config_getString(HTAGS_OPTIONS);
  QCString projectName = Config_getString(PROJECT_NAME);
  QCString projectNumber = Config_getString(PROJECT_NUMBER);

  if (inputSource.empty())
  {
    g_inputDir.setPath(Dir::currentDirPath());
  }
  else if (inputSource.size()==1)
  {
    g_inputDir.setPath(inputSource.back());
    if (!g_inputDir.exists())
      err("Cannot find directory %s. "
          "Check the value of the INPUT tag in the configuration file.\n",
          inputSource.back().c_str()
         );
  }
  else
  {
    err("If you use USE_HTAGS then INPUT should specify a single directory.\n");
    return FALSE;
  }

  /*
   * Construct command line for htags(1).
   */
  QCString commandLine = " -g -s -a -n ";
  if (!quiet)   commandLine += "-v ";
  if (warnings) commandLine += "-w ";
  if (!htagsOptions.isEmpty())
  {
    commandLine += ' ';
    commandLine += htagsOptions;
  }
  if (!projectName.isEmpty())
  {
    commandLine += "-t \"";
    commandLine += projectName;
    if (!projectNumber.isEmpty())
    {
      commandLine += '-';
      commandLine += projectNumber;
    }
    commandLine += "\" ";
  }
  commandLine += " \"" + htmldir + "\"";
  std::string oldDir = Dir::currentDirPath();
  Dir::setCurrent(g_inputDir.absPath());
  //printf("CommandLine=[%s]\n",commandLine.data());
  Portable::sysTimerStart();
  bool result=Portable::system("htags",commandLine,FALSE)==0;
  if (!result)
  {
    err("Problems running %s. Check your installation\n", "htags");
  }
  Portable::sysTimerStop();
  Dir::setCurrent(oldDir);
  return result;
}


/*! load filemap and make index.
 *  \param htmlDir of HTML directory generated by htags(1).
 *  \retval TRUE success
 *  \retval FALSE error
 */
bool Htags::loadFilemap(const QCString &htmlDir)
{
  QCString fileMapName = htmlDir+"/HTML/FILEMAP";
  FileInfo fi(fileMapName.str());
  /*
   * Construct FILEMAP dictionary.
   *
   * In FILEMAP, URL includes 'html' suffix but we cut it off according
   * to the method of FileDef class.
   *
   * FILEMAP format:
   * <NAME>\t<HREF>.html\n
   * QDICT:
   * dict[<NAME>] = <HREF>
   */
  if (fi.exists() && fi.isReadable())
  {
    std::ifstream f(fileMapName.str(),std::ifstream::in);
    if (f.is_open())
    {
      std::string lineStr;
      while (getline(f,lineStr))
      {
        QCString line = lineStr;
        //printf("Read line: %s",line.data());
        int sep = line.find('\t');
        if (sep!=-1)
        {
          QCString key   = line.left(sep).stripWhiteSpace();
          QCString value = line.mid(sep+1).stripWhiteSpace();
          int ext=value.findRev('.');
          if (ext!=-1) value=value.left(ext); // strip extension
          g_symbolMap.insert(std::make_pair(key.str(),value.str()));
          //printf("Key/Value=(%s,%s)\n",key.data(),value.data());
        }
      }
      return true;
    }
    else
    {
      err("file %s cannot be opened\n",fileMapName.data());
    }
  }
  return false;
}

/*! convert path name into the url in the hypertext generated by htags.
 *  \param path path name
 *  \returns URL NULL: not found.
 */
QCString Htags::path2URL(const QCString &path)
{
  QCString url,symName=path;
  QCString dir = g_inputDir.absPath();
  int dl=dir.length();
  if ((int)symName.length()>dl+1)
  {
    symName = symName.mid(dl+1);
  }
  if (!symName.isEmpty())
  {
    auto it = g_symbolMap.find(symName.str());
    //printf("path2URL=%s symName=%s result=%p\n",path.data(),symName.data(),result);
    if (it!=g_symbolMap.end())
    {
      url = QCString("HTML/") + it->second;
    }
  }
  return url;
}

