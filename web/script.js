console.log("JS LOADED");

const list = document.getElementById("history");

if (!list) {
  document.body.innerHTML += "<p>HISTORY ELEMENT NOT FOUND</p>";
} else {
  document.body.innerHTML += "<p>HISTORY ELEMENT FOUND</p>";
}

fetch("versions.json")
  .then(res => {
    console.log("FETCH RESPONSE:", res.status);
    return res.json();
  })
  .then(data => {
    document.body.innerHTML += "<p>JSON FETCHED</p>";

    data.forEach(v => {
      const li = document.createElement("li");
      li.textContent = `${v.version} | ${v.message}`;
      list.appendChild(li);
    });
  })
  .catch(err => {
    document.body.innerHTML += "<p>FETCH FAILED</p>";
    console.error(err);
  });
