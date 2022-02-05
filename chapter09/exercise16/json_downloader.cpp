#include "json_downloader.hpp"

void Json_downloader::erase(const char C, const char A) {
   for (unsigned i = 0; i < doc.size(); i++) {
      if (C == doc[i] || A == doc[i]) {
         doc.erase(doc.begin() + i);
         if (A == doc[i]) {
            doc.erase(doc.begin() + i);
            while (i < doc.size() && doc[i] != A) 
               doc.erase(doc.begin() + i);
            if (i < doc.size() && A == doc[i]) 
               doc.erase(doc.begin() + i);
         }
      }
   }
}

void Json_downloader::modify_document() {
   size_t first = doc.find("{");
   doc = doc.substr(first);
   size_t last = doc.rfind("}");
   doc = doc.substr(0, last + 1);
   erase('\r', '\n');
}
