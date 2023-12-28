<%@ page contentType="text/html; charset=UTF-8" pageEncoding="UTF-8" %>
<!DOCTYPE html>
<html>
<head>
  <title>JSP - Hello World</title>
</head>
<body>
<h1><%= "Hello World!" %></h1>
<br/>
<a href="hello-servlet">Hello Servlet</a>
<form action="hello-servlet" method="POST">
  <label for="text-input">Enter Belarusian for some magic</label><input type="text" id="text-input"
                                                                        name="text-input" value="123">
  <input type="submit" value="Submit">
</form>
</body>
</html>