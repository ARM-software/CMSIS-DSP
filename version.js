//--- list of versions ---
const versions = {
    "main": "1.16.3-dev45",
    "latest": "1.16.2",
    "v1.16.1": "1.16.1",
    "v1.16.0": "1.16.0",
    "v1.15.0": "1.15.0",
    "v1.14.4": "1.14.4",
    "v1.14.3": "1.14.3",
    "v1.14.2": "1.14.2",
    "v1.14.1": "1.14.1",
    "v1.14.0": "1.14.0",
    "v1.12.0": "1.12.0",
    "v1.11.0": "1.11.0",
    "v1.10.1": "1.10.1"
}
//--- list of versions ---

var script = document.currentScript
if (script && script.src) {
  var scriptUrl = new URL(script.src);
  var docUrl = new URL(document.URL);
  var baseUrl = new URL(scriptUrl)
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
              label += " ("+version+")"
          }
          label = "Version " + label
          document.write('    <a href="'+urlForVersion(docUrl, version)+'">'+label+'</a>');
      }
      document.write('  </div>');
  };
} else {
  function writeVersionDropdown() {}
}

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
