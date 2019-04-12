<!--
    located /oauth/write.jsp 
-->
<%@ page import="java.net.URLEncoder" %>
<%@ page import="java.net.URL" %>
<%@ page import="java.net.HttpURLConnection" %>
<%@ page import="java.io.BufferedReader" %>
<%@ page import="java.io.InputStreamReader" %>
<%@ page import="java.io.DataOutputStream"%>
<%@ page import="javax.servlet.http.HttpServletRequest"%>
<%@ page import="java.io.File"%>
<%@ page import="java.util.*"%>
<%@ page import="java.util.ArrayList"%>
<%@ page import="java.util.List"%>
<%@ page import="java.io.*"%>
<%@ page import="java.net.URLConnection"%>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<html>
  <head>
    <title>네이버로그인</title>
  </head>
  <body>
<%!
    public class MultipartUtil {

        private String boundary;
        private String LINE_FEED = "\r\n";
        private HttpURLConnection con;
        private OutputStream outputStream;
        private PrintWriter writer;

        public MultipartUtil (String apiURL) throws IOException  {
            boundary = "---" + System.currentTimeMillis() + "---";
            System.out.println("MultipartUtil boundary = " + boundary);
            URL url = new URL(apiURL);
            con = (HttpURLConnection)url.openConnection();
            con.setUseCaches(false);
            con.setDoOutput(true);
            con.setDoInput(true);
            con.setRequestProperty("Content-Type", "multipart/form-data; boundary=" + boundary);
        }

        public void readyToConnect() throws IOException {
            outputStream = con.getOutputStream();
            writer = new PrintWriter(new OutputStreamWriter(outputStream, "UTF-8"), true);
        }

        public void addFilePart(String fieldName, File uploadFile) throws IOException {
            String fileName = uploadFile.getName();
            writer.append("--" + boundary).append(LINE_FEED);
            writer.append("Content-Disposition: form-data; name=\"" + fieldName + "\"; filename=\"" + fileName + "\"").append(LINE_FEED);
            writer.append("Content-Type: " + URLConnection.guessContentTypeFromName(fileName)).append(LINE_FEED);
            writer.append("Content-Transfer-Encoding: binary").append(LINE_FEED);
            writer.append(LINE_FEED);
            writer.flush();

            FileInputStream inputStream = new FileInputStream(uploadFile);
            byte[] buffer = new byte[4096];
            int bytesRead = -1;
            while ((bytesRead = inputStream.read(buffer)) != -1) {
                outputStream.write(buffer, 0, bytesRead);
            }
            outputStream.flush();
            inputStream.close();

            writer.append(LINE_FEED);
            writer.flush();
        }

        public void addFormField(String name, String value) {
            writer.append("--" + boundary).append(LINE_FEED);
            writer.append("Content-Disposition: form-data; name=\"" + name + "\"").append(LINE_FEED);
            writer.append("Content-Type: text/plain; charset=UTF-8").append(LINE_FEED);
            writer.append(LINE_FEED);
            writer.append(value).append(LINE_FEED);
            writer.flush();
        }

        public void addHeaderField(String name, String value) {
            con.setRequestProperty(name, value);
        }

        public List finish() throws IOException {
            List response = new ArrayList();

            writer.append(LINE_FEED).flush();
            writer.append("--" + boundary + "--").append(LINE_FEED);
            writer.close();

            int status = con.getResponseCode();
            if (status == HttpURLConnection.HTTP_OK) {
                BufferedReader reader = new BufferedReader(new InputStreamReader(con.getInputStream()));
                String line = null;
                while ((line = reader.readLine()) != null) {
                    response.add(line);
                }
                reader.close();
                con.disconnect();
            } else {
                throw new IOException("Server returned non-OK status: " + status);
            }
            return response;
        }
    }
%>
<%
    request.setCharacterEncoding("EUC-KR");
    String token = request.getParameter("access_token");
    String dateFrom = request.getParameter("dateFrom");
    String header = "Bearer " + token; 
    System.out.println("header : "+header);

    String[] fromDateParisingList = dateFrom.split("-");
    for(String s:fromDateParisingList) {
        System.out.println("CHECK POINT: "+s);
    }
    File dir = new File("directory"+fromDateParisingList[1]+"월\\"+fromDateParisingList[1]+"."+fromDateParisingList[2]);
    File[] listOfFiles = dir.listFiles();
    try {
        String clubid = "clubid";
        String menuid = "menuid"; 
        String cafeApiURL = "https://openapi.naver.com/v1/cafe/"+ clubid+"/menu/" + menuid + "/articles";    
        MultipartUtil mu = new MultipartUtil(cafeApiURL);
        mu.addHeaderField("Authorization", header);
        mu.readyToConnect();
        String innerContent = fromDateParisingList[0]+"년\\s"+fromDateParisingList[1]+"월\\s"+fromDateParisingList[2]+"일\\sTEST";
        System.out.println("CHECK POINT: "+innerContent);
        String subject = URLEncoder.encode(URLEncoder.encode(innerContent, "UTF-8"), "MS949");
        mu.addFormField("subject", subject);
        String content = URLEncoder.encode(URLEncoder.encode("dsad", "UTF-8"), "MS949");
        mu.addFormField("content", content);

        for(File file : listOfFiles) {
            mu.addFilePart("0", file);
            System.out.println("CHECK POINT: "+file.getName());
        }
        mu.addFormField("searchopen","false");
        mu.addFormField("replyyn","false");
        mu.addFormField("scrapyn","false");
        mu.addFormField("metoo","false");
        mu.addFormField("autosourcing","false");
        mu.addFormField("rclick","false");
        mu.addFormField("ccl","false");
        List responsed = mu.finish();
        System.out.println("SERVER REPLIED:");
        out.println("Successfully Write Posting");
    } catch (Exception e) {
        System.out.println(e);
        out.println("Failed Write Posting");
    }                
%>
  </body>
</html>