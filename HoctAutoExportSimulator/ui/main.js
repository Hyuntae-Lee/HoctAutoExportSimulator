var drawAngioImage = function(groupId, imageId) {
    var group = document.getElementById(groupId);
    var canvas = group.getElementsByTagName('canvas')[0];
    var ctx = canvas.getContext('2d');
    var img = new Image();
    img.onload = function() {
        ctx.drawImage(img, 0, 0, img.width, img.height, 0, 0, canvas.width, canvas.height);
    };
    img.src = external.getAngioImagePath(imageId);
}

var drawEnfaceImage = function(groupId) {
    var group = document.getElementById(groupId);
    var canvas = group.getElementsByTagName('canvas')[0];
    var ctx = canvas.getContext('2d');
    var img = new Image();
    img.onload = function() {
        ctx.drawImage(img, 0, 0, img.width, img.height, 0, 0, canvas.width, canvas.height);
    };
    img.src = external.getEnfaceImagePath();
}

var drawBScanImage = function(groupId, imageId) {
    var group = document.getElementById(groupId);
    var canvas = group.getElementsByTagName('canvas')[0];
    var ctx = canvas.getContext('2d');
    var img = new Image();
    img.onload = function() {
        ctx.drawImage(img, 0, 0, img.width, img.height, 0, 0, canvas.width, canvas.height);
    };
    img.src = external.getBScanImagePath(imageId);
}

var drawPatientInfo = function(name, id) {
    var group = document.getElementById('patient_info');
    var labelList = group.getElementsByTagName('label');
    labelList[0].innerText = name;
    labelList[1].innerText = id;
}

var drawMeasInfo = function(resultInfo, measInfo) {
    var group = document.getElementById('meas_info');
    var labelList = group.getElementsByTagName('label');
    labelList[0].innerText = resultInfo;
    labelList[1].innerText = measInfo;
}
