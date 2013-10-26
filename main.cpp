#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>

std::string tempOut;
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{

    /*std::ofstream *out = (std::ofstream*)stream;
    out->open("scribeout1.txt");

     out->write((char*)buffer, size);
     return nmemb*size;*/
    tempOut += (char*)buffer;
    //tempOut.push_back((char*)buffer);
    return size*nmemb;
}


int main(int argc, char *argv[])
{
    CURL *curl;
 CURLcode res;

   curl_slist *slist=NULL;
   //std::string tempOut = "blah";


   std::ofstream out;
   //out = fopen ("scribeout.txt", "w");
   out.open("scribeout.txt");

 slist = curl_slist_append(slist, "X-Mashape-Authorization: SDmDf8mq9jguHZbyxcCAIBqhvdWbWJ6n");
 curl_global_init(CURL_GLOBAL_DEFAULT);

 curl = curl_easy_init();
 if(curl) {
   curl_easy_setopt(curl, CURLOPT_URL, "https://teemojson.p.mashape.com/misc/summoner-name/na/54353;15838;141;3467;2;333333");
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
   //curl_easy_setopt(curl, CURLOPT_WRITEDATA, out);

#ifdef SKIP_PEER_VERIFICATION
   /*
    * If you want to connect to a site who isn't using a certificate that is
    * signed by one of the certs in the CA bundle you have, you can skip the
    * verification of the server's certificate. This makes the connection
    * A LOT LESS SECURE.
    *
    * If you have a CA cert for the server stored someplace else than in the
    * default bundle, then the CURLOPT_CAPATH option might come handy for
    * you.
    */
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

#ifdef SKIP_HOSTNAME_VERIFICATION
   /*
    * If the site you're connecting to uses a different host name that what
    * they have mentioned in their server certificate's commonName (or
    * subjectAltName) fields, libcurl will refuse to connect. You can skip
    * this check, but this will make the connection less secure.
    */
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

   /* Perform the request, res will get the return code */
   res = curl_easy_perform(curl);
   /* Check for errors */
   if(res != CURLE_OK)
     fprintf(stderr, "curl_easy_perform() failed: %s\n",
             curl_easy_strerror(res));

   /* always cleanup */
   curl_easy_cleanup(curl);
 }


 std::cout<<tempOut<<std::endl;
 out<<tempOut;
 out.close();
 curl_global_cleanup();


 QApplication a(argc, argv);
 MainWindow w;
 w.show();

    return a.exec();
}


