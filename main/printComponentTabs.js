var strgURL =   location.pathname;                      // path of current component

// constuctor for the array of objects
function tabElement(id, folderName, tabTxt )  {
	this.id = id;                                       // elementID as needed in html;
	this.folderName = folderName;                       // folder name of the component
    this.tabTxt = tabTxt;                               // Text displayed as menu on the web
	this.currentListItem = '<li id="' + this.id + '" class="current"> <a href="../..' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>';
	this.listItem = '<li id="' + this.id + '"> <a href="../..' + this.folderName + 'index.html"><span>' + this.tabTxt + '</span></a></li>';
};

// array of objects
var arr = [];

// fill array
// arr.push( new    tabElement( "GEN",    "/General/html/", "General"));

// write tabs
// called from the header file.
function writeComponentTabs()  {
  for ( var i=0; i < arr.length; i++ ) {
    if (strgURL.search(arr[i].folderName) > 0) {                    // if this is the current folder
      document.write(arr[i].currentListItem);                       // then print and highlight the tab
    } else {
      document.write(arr[i].listItem);                              // else, print the tab
    }
  }
};
