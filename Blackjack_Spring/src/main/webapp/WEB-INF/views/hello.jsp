<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>

<ul>
    <c:each items="{boards}">
        <li><c:out>${title}</c:out></li>
    </c:each>
</ul>
</body>
</html>