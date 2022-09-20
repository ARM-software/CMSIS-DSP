//--- list of versions ---
const versions = {
    "main": "1.13.1-dev2",
    "latest": "1.12.0",
    "v1.11.0": "1.11.0",
    "v1.10.1": "1.10.1"
}
//--- list of versions ---

var scripts = document.getElementsByTagName("script"),
scriptUrl = new URL(scripts[scripts.length-1].src);
docUrl = new URL(document.URL);
baseUrl = new URL(scriptUrl)
baseUrl.pathname = baseUrl.pathname.split('/').slice(0,-1).join("/")

function urlForVersion(url, version) {
    url = new URL(url);
    pathname = url.pathname.replace(baseUrl.pathname, "");
    parts = pathname.split("/");
    parts[1] = version;
    url.pathname = baseUrl.pathname + parts.join("/");
    return url
}

function writeVersionDropdown() {
    currentVersion = document.currentScript.parentNode.innerText;
    document.currentScript.parentNode.classList.add("dropdown");
    document.currentScript.parentNode.innerText = "";
    document.write('  <span onclick="myFunction()" class="dropbtn">'+currentVersion+'</span>');
    document.write('  <div id="myDropdown" class="dropdown-content">'); 
    for(var version in versions) {
        var label = versions[version];
        if (label != version) {
            label += "("+version+")"
        }
        label = "Version " + label
        document.write('    <a href="'+urlForVersion(docUrl, version)+'">'+label+'</a>');
    }
    document.write('  </div>');
};

/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function myFunction() {
  document.getElementById("myDropdown").classList.toggle("show");
}

// Close the dropdown menu if the user clicks outside of it
window.onclick = function(event) {
  if (!event.target.matches('.dropbtn')) {
    var dropdowns = document.getElementsByClassName("dropdown-content");
    var i;
    for (i = 0; i < dropdowns.length; i++) {
      var openDropdown = dropdowns[i];
      if (openDropdown.classList.contains('show')) {
        openDropdown.classList.remove('show');
      }
    }
  }
}
