// Campus Connect - Front Page Launcher
// Running this program creates front_page.html and opens it in your default browser.
// No graphics.h / conio.h needed.

#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

int main()
{
    const char *fileName = "front_page.html";

    ofstream out(fileName);
    if (!out)
    {
        cout << "Could not create " << fileName << endl;
        return 1;
    }

    out << R"HTML(<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Campus Connect</title>
<link href="https://fonts.googleapis.com/css2?family=Pacifico&family=Playfair+Display:wght@700&display=swap" rel="stylesheet">
<style>
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body { background: #fff; font-family: Georgia, "Times New Roman", serif; min-height: 100vh; padding: 20px; }
  .frame {
    min-height: calc(100vh - 40px);
    border: 4px double #1a3fd0;
    border-radius: 6px;
    display: flex; flex-direction: column; align-items: center; justify-content: center;
    text-align: center; padding: 30px 20px;
  }
  .screen { display: none; flex-direction: column; align-items: center; gap: 14px; width: 100%; }
  .screen.active { display: flex; animation: fade .6s ease-out; }
  .top   { color: #1a3fd0; font-size: 2rem; font-weight: bold; letter-spacing: 3px; }
  .phase { color: #1a3fd0; font-size: 1.4rem; letter-spacing: 2px; }
  .project { color: #d62020; font-size: 3.4rem; font-weight: bold; letter-spacing: 4px; margin: 14px 0 4px; }
  .sub  { color: #111; font-size: 1.2rem; }
  .line { width: 220px; height: 3px; background: #1a3fd0; margin: 8px 0; }
  h2 { color: #1a3fd0; font-size: 2rem; margin-bottom: 6px; }
  p.msg { color: #111; font-size: 1.15rem; max-width: 520px; line-height: 1.6; }
  .options { display: flex; flex-wrap: wrap; justify-content: center; gap: 18px; margin-top: 18px; max-width: 1000px; }
  .opt {
    width: 230px; padding: 22px 12px;
    background: #fff; color: #1a3fd0;
    border: 3px solid #1a3fd0; border-radius: 14px;
    font-size: 1.15rem; font-family: inherit; font-weight: bold; cursor: pointer;
    transition: transform .15s, background .15s, color .15s;
  }
  .opt:hover { background: #1a3fd0; color: #fff; transform: translateY(-4px); }
  .opt .icon { display: block; font-size: 2rem; margin-bottom: 8px; }

  /* University Map: a different, wide, highlighted option */
  .map-opt {
    width: 100%; max-width: 760px; margin-top: 8px; padding: 20px 24px;
    background: linear-gradient(135deg, #d62020, #8e1010);
    color: #fff; border: none; border-radius: 40px;
    font-size: 1.3rem; font-family: inherit; font-weight: bold; letter-spacing: 2px;
    cursor: pointer; box-shadow: 0 6px 16px rgba(214, 32, 32, .35);
    transition: transform .15s, box-shadow .15s;
  }
  .map-opt:hover { transform: scale(1.03); box-shadow: 0 10px 22px rgba(214, 32, 32, .5); }

  .route-opt {
    margin-top: 10px; padding: 18px 34px;
    background: #1a3fd0; color: #fff; border: none; border-radius: 40px;
    font-size: 1.2rem; font-family: inherit; font-weight: bold; cursor: pointer;
    transition: transform .15s, background .15s;
  }
  .route-opt:hover { background: #d62020; transform: scale(1.05); }

  .back {
    margin-top: 22px; padding: 10px 28px;
    background: #555; color: #fff; border: none; border-radius: 30px;
    font-size: 1rem; font-family: inherit; cursor: pointer;
  }
  .back:hover { background: #222; }
  .dept { color: #111; font-size: 1.05rem; margin-top: 24px; line-height: 1.6; }
  /* Announcements and Clubs */
  .panels { display: flex; flex-wrap: wrap; gap: 24px; width: 100%; max-width: 1200px; align-items: flex-start; margin-top: 10px; }
  .panel {
    flex: 1 1 460px; min-width: 0; text-align: left;
    background: #fff; border: 3px solid #1a3fd0; border-radius: 16px;
    box-shadow: 0 8px 22px rgba(26, 63, 208, .25);
    padding: 20px 24px; max-height: 62vh; overflow-y: auto;
    animation: popIn .45s ease-out;
  }
  .panel h3 { color: #d62020; font-size: 1.5rem; text-align: center; margin-bottom: 12px; }
  .phase-box { border-left: 5px solid #1a3fd0; background: #f3f6ff; border-radius: 8px; padding: 10px 14px; margin-bottom: 14px; }
  .phase-box h4 { color: #1a3fd0; font-size: 1.2rem; margin-bottom: 6px; }
  .phase-box ul { margin-left: 20px; }
  .phase-box li { color: #111; font-size: 1rem; line-height: 1.5; margin-bottom: 8px; }
  .phase-box li b { color: #1a3fd0; }
  .panel p.msg { text-align: center; margin: 30px auto; }
  @keyframes popIn { from { opacity: 0; transform: scale(.9); } to { opacity: 1; transform: scale(1); } }

  /* Announcements & Clubs */
  .split { display: flex; flex-wrap: wrap; gap: 24px; width: 100%; max-width: 1200px; align-items: flex-start; margin-top: 10px; }
  .panel {
    flex: 1; min-width: 320px; text-align: left;
    border: 3px solid #1a3fd0; border-radius: 14px; padding: 16px 18px;
    max-height: 62vh; overflow-y: auto; background: #fff;
    box-shadow: 0 6px 16px rgba(26, 63, 208, .15);
  }
  .panel h3 { color: #d62020; font-size: 1.4rem; margin-bottom: 10px; text-align: center; }
  .panel details { border: 2px solid #1a3fd0; border-radius: 10px; margin-bottom: 10px; overflow: hidden; }
  .panel summary {
    cursor: pointer; padding: 10px 14px; background: #1a3fd0; color: #fff;
    font-weight: bold; font-size: 1.1rem; list-style: none;
  }
  .panel summary::-webkit-details-marker { display: none; }
  .panel summary::before { content: "\25B8  "; }
  .panel details[open] summary::before { content: "\25BE  "; }
  .panel details[open] summary { background: #d62020; }
  .panel ul { padding: 12px 14px 12px 34px; color: #111; font-size: 1rem; line-height: 1.55; }
  .panel li { margin-bottom: 10px; }
  .panel li b { color: #1a3fd0; }
  .panel p.soon { color: #444; text-align: center; padding: 30px 10px; font-size: 1.1rem; }
  .know-btn {
    margin-top: 14px; padding: 16px 46px;
    background: linear-gradient(135deg, #d62020, #8e1010); color: #fff; border: none; border-radius: 40px;
    font-size: 1.25rem; font-family: inherit; font-weight: bold; letter-spacing: 2px; cursor: pointer;
    box-shadow: 0 6px 16px rgba(214, 32, 32, .35); transition: transform .15s;
  }
  .know-btn:hover { transform: scale(1.05); }

  /* Full-page university map */
  .map-full {
    display: none; position: fixed; top: 0; left: 0; width: 100%; height: 100%;
    background: #000; z-index: 50; align-items: center; justify-content: center;
  }
  .map-full.show { display: flex; }
  .map-full img { max-width: 100%; max-height: 100%; object-fit: contain; }
  .map-back {
    position: absolute; top: 16px; right: 20px; z-index: 51;
    padding: 10px 26px; background: #d62020; color: #fff; border: 2px solid #fff; border-radius: 30px;
    font-size: 1.05rem; font-family: inherit; font-weight: bold; cursor: pointer;
    box-shadow: 0 4px 12px rgba(0, 0, 0, .5);
  }
  .map-back:hover { background: #a81414; }
  .map-missing { display: none; position: absolute; color: #fff; font-size: 1.2rem; text-align: center; padding: 20px; }

  /* Contact (lost items) */
  .contact-title {
    font-family: "Pacifico", "Brush Script MT", cursive; font-weight: normal;
    font-size: 3rem; color: #d62020; text-shadow: 3px 3px 0 #c9d3ff; margin-bottom: 0;
  }
  .contacts {
    display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 18px;
    width: 100%; max-width: 1000px; margin-top: 16px;
  }
  .contact {
    border: 3px solid #1a3fd0; border-radius: 14px; padding: 20px 14px; background: #fff;
    box-shadow: 0 6px 16px rgba(26, 63, 208, .15);
  }
  .cname { font-family: "Playfair Display", Georgia, serif; font-size: 1.6rem; color: #1a3fd0; font-weight: bold; margin-bottom: 10px; }
  .contact a { color: #d62020; font-weight: bold; font-size: 1.05rem; word-break: break-all; text-decoration: none; border-bottom: 2px dashed #d62020; }
  .contact a:hover { color: #1a3fd0; border-color: #1a3fd0; }
  .face { color: #444; font-style: italic; font-size: 1.1rem; }

  .map-btn {
    margin-top: 6px; padding: 14px 34px; background: #d62020; color: #fff; border: none; border-radius: 40px;
    font-size: 1.1rem; font-family: inherit; font-weight: bold; cursor: pointer; transition: transform .15s, background .15s;
  }
  .map-btn:hover { background: #a81414; transform: scale(1.05); }

  /* Login */
  .login-box { width: 100%; max-width: 440px; margin-top: 16px; display: flex; flex-direction: column; gap: 14px; }
  .login-box input {
    padding: 14px 18px; font-size: 1.1rem; font-family: inherit;
    border: 3px solid #1a3fd0; border-radius: 12px; outline: none; text-align: center;
  }
  .login-box input:focus { border-color: #d62020; }
  .login-btn {
    padding: 14px; background: #1a3fd0; color: #fff; border: none; border-radius: 40px;
    font-size: 1.15rem; font-family: inherit; font-weight: bold; cursor: pointer;
    transition: background .15s, transform .15s;
  }
  .login-btn:hover { background: #d62020; transform: scale(1.03); }
  .error { color: #d62020; font-weight: bold; font-size: 1.1rem; min-height: 1.4em; }

  /* "Valid User" stamp, upper right corner */
  .stamp {
    display: none; position: fixed; top: 34px; right: 40px; z-index: 10;
    padding: 6px 18px; color: #0a8a2a; background: rgba(255, 255, 255, .9);
    border: 4px double #0a8a2a; border-radius: 10px;
    font-family: "Courier New", monospace; font-size: 1.2rem; font-weight: bold;
    letter-spacing: 2px; text-transform: uppercase; transform: rotate(-6deg);
  }
  .stamp.show { display: block; animation: stampIn .4s ease-out; }
  @keyframes stampIn { from { opacity: 0; transform: rotate(-6deg) scale(2.2); } to { opacity: 1; transform: rotate(-6deg) scale(1); } }

  @keyframes fade { from { opacity: 0; transform: translateY(-10px); } to { opacity: 1; transform: none; } }
  @media (max-width: 700px) { .project { font-size: 2.1rem; } .top { font-size: 1.4rem; } }
</style>
</head>
)HTML";
    out << R"HTML(
<style>
  /* Clubs */
  .club-list { display: flex; flex-direction: column; gap: 10px; }
  .club-btn {
    display: flex; align-items: center; gap: 12px; width: 100%;
    padding: 14px 16px; background: #fff; color: #1a3fd0;
    border: 2px solid #1a3fd0; border-radius: 12px;
    font-size: 1.1rem; font-family: inherit; font-weight: bold; text-align: left; cursor: pointer;
    transition: transform .15s, background .15s, color .15s;
  }
  .club-btn:hover { background: #1a3fd0; color: #fff; transform: translateX(6px); }
  .club-btn .cicon { font-size: 1.5rem; }
  .club-note {
    color: #111; font-size: 1.2rem; max-width: 640px; line-height: 1.6; margin: 6px 0 4px;
  }
  .club-footer {
    font-family: "Pacifico", "Brush Script MT", cursive; font-size: 1.5rem; line-height: 1.8;
    color: #d62020; max-width: 760px; margin-top: 18px; padding: 16px 22px;
    border-top: 3px solid #1a3fd0; border-bottom: 3px solid #1a3fd0;
    text-shadow: 2px 2px 0 #c9d3ff;
  }
  .contact .cmail-label { color: #444; font-size: .9rem; margin-bottom: 4px; }
</style>
</head>
)HTML";
    out << R"HTML(
<body>
<div class="stamp" id="stamp">&#10004; Valid User</div>
<div class="frame">

  <!-- LOGIN -->
  <div class="screen active" id="login">
    <div class="project">CAMPUS CONNECT</div>
    <div class="line"></div>
    <h2>Login</h2>
    <p class="msg">Enter your university email ID to continue.</p>
    <div class="login-box">
      <input type="text" id="email" placeholder="e.g. 2510380056@geu.ac.in" autocomplete="off">
      <button class="login-btn" onclick="login()">Login</button>
      <div class="error" id="loginError"></div>
    </div>
  </div>

  <!-- HOME -->
  <div class="screen" id="home">
    <div class="top">PROJECT-BASED LEARNING</div>
    <div class="phase">PHASE-I EVALUATION</div>
    <div class="line"></div>
    <div class="project">CAMPUS CONNECT</div>
    <div class="sub">A Smart Digital Platform for Campus Communication</div>
    <div class="sub">Student Collaboration and Event Management</div>

    <div class="options">
      <button class="opt" onclick="show('lost')"><span class="icon">&#128269;</span>Something you lost</button>
      <button class="opt" onclick="show('navigate')"><span class="icon">&#128205;</span>Navigate the campus</button>
      <button class="opt" onclick="show('find')"><span class="icon">&#128100;</span>Find someone</button>
      <button class="opt" onclick="show('announce')"><span class="icon">&#128227;</span>Announcements and Clubs</button>
    </div>

    <div class="dept">
      TEAM ID : XXXX<br>
      Department of Computer Science &amp; Engineering<br>
      Graphic Era (Deemed to be University) | Academic Session 2026-27
    </div>
  </div>

  <!-- ANNOUNCEMENTS AND CLUBS -->
  <div class="screen" id="announce">
    <h2>Announcements and Clubs</h2>
    <div class="split">

      <div class="panel" style="flex: 1 1 420px;">
        <h3>&#128227; Announcements</h3>

        <details open>
          <summary>Upcoming Events</summary>
          <ul>
            <li><b>Hands-on Workshop on Internet of Things (IoT):</b> Officially scheduled for October 13, 2026.</li>
            <li><b>Project-Based Learning (PBL) Phase-II Evaluation:</b> Set to take place on October 19, 2026.</li>
            <li><b>Ph.D. Admission Notifications (January 2027 Cycle):</b> Registration guidelines, eligibility criteria, and timeline details are currently being released for the upcoming winter research intake.</li>
            <li><b>Mid-Semester Back Practical Examinations:</b> Scheduled throughout October 2026 for clearing active backlogs.</li>
            <li><b>Major Project Synopsis Approval (7th Semester):</b> Makeup sessions and absolute evaluation listings for absentees or rejected synopsis submissions were updated through late September and early October 2026.</li>
          </ul>
        </details>

        <details>
          <summary>Recent Past Announcements</summary>
          <ul>
            <li><b>End-Term Academic Assessments:</b> Release of the August&ndash;September 2026 End-Term Examination Date Sheets alongside special End-Semester Practical Exam schedules for internship-tracked students in mid-September.</li>
            <li><b>GECET 2026 Entrance Examination:</b> The last online remote-proctored institutional entrance phase concluded its applications on July 22, 2026, with the test administered on July 25, 2026.</li>
            <li><b>Branch Upgradation &amp; Session Launch:</b> Counselling notices for B.Tech III Semester Branch Changes (Batch 2025&ndash;2029) and the standard Commencement notice for the 1st Year Academic Session 2026&ndash;27.</li>
            <li><b>Online Education (ODL) Support:</b> Special Learning Management System (LMS) quiz circulars and ODL/OL Back Examination schedules were distributed in June and July 2026.</li>
          </ul>
        </details>

        <details>
          <summary>Past Milestones</summary>
          <ul>
            <li><b>13th Convocation Ceremony:</b> Successfully celebrated on May 24, 2026, conferring degrees upon graduating batches.</li>
            <li><b>Placement Felicitation Ceremony 2026:</b> Conducted on May 9, 2026, honoring top-placed students across tech and management domains.</li>
            <li><b>BIOCOM 2026:</b> A premier national conference tracking computational biology and sustainable food ecosystems held on May 6, 2026.</li>
            <li><b>Grafest 2026:</b> The university's hallmark annual socio-cultural festival was hosted from April 13&ndash;14, 2026, featuring prominent guest performers and sub-competitions.</li>
            <li><b>World Record Event (Hospitality Management):</b> Achieved a global milestone on April 29, 2026, by preparing a record breaking 554 unique paratha varieties on campus.</li>
          </ul>
        </details>
      </div>

      <div class="panel" style="flex: 1 1 320px;">
        <h3>&#127942; Clubs</h3>
        <div class="club-list" id="clubList"></div>
      </div>

    </div>

    <button class="know-btn" onclick="window.open('https://geu.ac.in/academics/notices', '_blank', 'noopener')">KNOW MORE</button>
    <button class="back" onclick="show('home')">&larr; Back to Home</button>
  </div>

  <!-- CLUB DETAIL -->
  <div class="screen" id="club">
    <h2 class="contact-title" id="clubTitle"></h2>
    <p class="club-note">Contact any of them to be added and confirm your membership for the selected club.</p>
    <div class="contacts">
      <div class="contact"><div class="cname">Anshul Negi</div><div class="cmail-label">Mail ID</div><a href="mailto:2510011872@geu.ac.in?subject=Club%20Membership">2510011872@geu.ac.in</a></div>
      <div class="contact"><div class="cname">Dev Dhoundial</div><div class="cmail-label">Mail ID</div><a href="mailto:2510380056@geu.ac.in?subject=Club%20Membership">2510380056@geu.ac.in</a></div>
      <div class="contact"><div class="cname">Sourav Bisht</div><div class="cmail-label">Mail ID</div><a href="mailto:2510011386@geu.ac.in?subject=Club%20Membership">2510011386@geu.ac.in</a></div>
      <div class="contact"><div class="cname">SG</div><div class="face">Meet him to talk to him</div></div>
    </div>
    <div class="club-footer">You will be notified very early once you send your application mail. Kindly add your WhatsApp number, later on we will notify you very soon.</div>
    <button class="back" onclick="show('announce')">&larr; Back to Clubs</button>
  </div>

  <!-- SOMETHING YOU LOST -->
  <div class="screen" id="lost">
    <h2 class="contact-title">Contact Us for Help</h2>
    <p class="msg">Lost something on campus? Click an email ID below and write to us about your lost item.</p>
    <div class="contacts">
      <div class="contact"><div class="cname">Ansul Negi</div><a href="mailto:2510011872@geu.ac.in?subject=Lost%20Item">2510011872@geu.ac.in</a></div>
      <div class="contact"><div class="cname">Sourav Bist</div><a href="mailto:2510011386@geu.ac.in?subject=Lost%20Item">2510011386@geu.ac.in</a></div>
      <div class="contact"><div class="cname">Dev Dhoundial</div><a href="mailto:2510380056@geu.ac.in?subject=Lost%20Item">2510380056@geu.ac.in</a></div>
      <div class="contact"><div class="cname">SG</div><div class="face">He only talks face to face</div></div>
    </div>
    <button class="back" onclick="show('home')">&larr; Back to Home</button>
  </div>

  <!-- FIND SOMEONE -->
  <div class="screen" id="find">
    <h2>Find Someone</h2>
    <p class="msg">Who are you looking for?</p>
    <div class="options" id="findOptions"></div>
    <button class="back" onclick="show('home')">&larr; Back to Home</button>
  </div>

  <!-- NAVIGATE THE CAMPUS -->
  <div class="screen" id="navigate">
    <h2>Navigate the Campus</h2>
    <p class="msg">Where would you like to go?</p>
    <div class="options" id="placeOptions"></div>
    <button class="map-opt" onclick="openMap()">&#128506; UNIVERSITY MAP</button>
    <button class="back" onclick="show('home')">&larr; Back to Home</button>
  </div>

  <!-- A SELECTED PLACE -->
  <div class="screen" id="place">
    <h2 id="placeTitle"></h2>
    <p class="msg" id="placeMsg">What would you like to do?</p>
    <button class="route-opt" id="routeBtn">&#128739; Route Search &ndash; Shortest Path</button>
    <button class="map-btn" id="placeMap" onclick="openMap()" style="display:none"></button>
    <button class="back" onclick="show('navigate')">&larr; Back</button>
  </div>

  <!-- GENERIC INFO SCREEN -->
  <div class="screen" id="info">
    <h2 id="infoTitle"></h2>
    <p class="msg" id="infoMsg"></p>
    <button class="map-btn" id="infoMap" onclick="openMap()" style="display:none"></button>
    <button class="back" id="infoBack">&larr; Back</button>
  </div>

</div>
)HTML";
    out << R"HTML(
<!-- FULL PAGE UNIVERSITY MAP -->
<div class="map-full" id="mapFull">
  <button class="map-back" onclick="closeMap()">&larr; Back</button>
  <img src="campus_map.jpeg" alt="University Map"
       onerror="this.style.display='none'; document.getElementById('mapMissing').style.display='block';">
  <div class="map-missing" id="mapMissing">Map image not found.<br>Place <b>campus_map.jpeg</b> in the same folder as front_page.html.</div>
</div>

<script>
  function openMap()  { document.getElementById('mapFull').classList.add('show'); }
  function closeMap() { document.getElementById('mapFull').classList.remove('show'); }
  document.addEventListener('keydown', function (e) { if (e.key === 'Escape') closeMap(); });

  function show(id) {
    var screens = document.querySelectorAll('.screen');
    for (var i = 0; i < screens.length; i++) screens[i].classList.remove('active');
    document.getElementById(id).classList.add('active');
  }

  // Login: only emails with "@geu" in the domain are allowed
  function login() {
    var email = document.getElementById('email').value.trim();
    var err = document.getElementById('loginError');
    if (/^[^\s@]+@geu\./i.test(email)) {
      err.textContent = '';
      document.getElementById('stamp').classList.add('show');
      show('home');
    } else {
      err.textContent = 'Invalid login';
    }
  }
  document.getElementById('email').addEventListener('keydown', function (e) {
    if (e.key === 'Enter') login();
  });

  // Generic message screen with a Back button that returns to "backTo"
  function info(title, msg, backTo, mapLabel) {
    document.getElementById('infoTitle').textContent = title;
    document.getElementById('infoMsg').textContent = msg;
    var mb = document.getElementById('infoMap');
    mb.style.display = mapLabel ? '' : 'none';
    if (mapLabel) mb.innerHTML = '&#128506; ' + mapLabel;
    document.getElementById('infoBack').onclick = function () { show(backTo); };
    show('info');
  }

  // Clubs
  var clubs = [
    ['\u{1F3F8}', 'Badminton Club'],
    ['\u265F\uFE0F', 'Chess Club'],
    ['\u{1F3C0}', 'Basketball Club'],
    ['\u26BD', 'Football Club'],
    ['\u{1F3CF}', 'Cricket Club'],
    ['\u{1F3B8}', 'Guitar Club']
  ];
  var clubBox = document.getElementById('clubList');
  clubs.forEach(function (c) {
    var b = document.createElement('button');
    b.className = 'club-btn';
    b.innerHTML = '<span class="cicon">' + c[0] + '</span>' + c[1];
    b.onclick = function () {
      document.getElementById('clubTitle').textContent = c[1];
      show('club');
    };
    clubBox.appendChild(b);
  });

  // Find someone options
  var findItems = [
    ['\u{1F468}\u200D\u{1F3EB}', 'Find any teacher'],
    ['\u{1F4CB}', 'Teacher details'],
    ['\u{1F9D1}', 'Find anyone else']
  ];
  var findBox = document.getElementById('findOptions');
  findItems.forEach(function (it) {
    var b = document.createElement('button');
    b.className = 'opt';
    b.innerHTML = '<span class="icon">' + it[0] + '</span>' + it[1];
    b.onclick = function () { info(it[1], 'This feature is coming soon.', 'find'); };
    findBox.appendChild(b);
  });

  // Campus places
  // d = text shown on the place page, r = shortest-path text (null = no Route Search button,
  // undefined = "coming soon"), m = label of the "See map" button (omit for none)
  var GATE      = 'Enter from Gate No. 1 and go towards Gate No. 2.';
  var GATE_R    = 'From B Tech Block towards Gate No. 1.';
  var CSIT_D    = 'Enter from the CSIT gate next to Gate No. 1. The higher building is the Paramedical Block and the widest one is the B Tech Block.';
  var CSIT_R    = 'Take the way near the Sports Ground and go through the underpass.';
  var FOUNTAIN  = 'Go straight from the B Tech Block, behind the fountain with statues.';

  var places = [
    {i: '\u{1F3EB}', n: 'CSIT Block', d: CSIT_D, r: CSIT_R},
    {i: '\u2615', n: 'Happiness Cafe', d: FOUNTAIN, r: null, m: 'See Map'},
    {i: '\u{1F4BB}', n: 'B TECH Block'},
    {i: '\u2696\uFE0F', n: 'SCHOOL OF LAW', d: GATE, r: GATE_R},
    {i: '\u2695\uFE0F', n: 'PARAMEDICAL Block', d: CSIT_D, r: CSIT_R},
    {i: '\u{1F3DB}\uFE0F', n: 'VISVESHWARAYA Block'},
    {i: '\u{1F6E0}\uFE0F', n: 'VISWAKARMA Block', d: GATE, r: GATE_R},
    {i: '\u{1F37D}\uFE0F', n: 'RAVI Canteen', d: FOUNTAIN, r: null, m: 'See Map'},
    {i: '\u{1F4DA}', n: 'SANTOSHANAND Library', d: 'Near the B Tech Block.', r: null, m: 'See Through Map'},
    {i: '\u{1F3E0}', n: 'Vidyarthi Bhavan'},
    {i: '\u{1F3F8}', n: 'Badminton and Basketball Ground'},
    {i: '\u{1F5A5}\uFE0F', n: 'BCA Building', d: GATE, r: GATE_R},
    {i: '\u{1F3E2}', n: 'Chanakya Block', d: GATE, r: GATE_R},
    {i: '\u{1F9ED}', n: 'Sardar Patel', d: 'Around the Sports Ground towards CSIT.', r: null},
    {i: '\u{1F393}', n: 'Silver Jubilee Convocation Centre', d: 'It is visible all around Subhash Nagar. Look around yourself.', r: null}
  ];
  var placeBox = document.getElementById('placeOptions');
  places.forEach(function (p) {
    var b = document.createElement('button');
    b.className = 'opt';
    b.innerHTML = '<span class="icon">' + p.i + '</span>' + p.n;
    b.onclick = function () {
      document.getElementById('placeTitle').textContent = p.n;
      document.getElementById('placeMsg').textContent = p.d || 'What would you like to do?';

      var rb = document.getElementById('routeBtn');
      if (p.r === null) {
        rb.style.display = 'none';
      } else {
        rb.style.display = '';
        rb.onclick = function () {
          if (p.r === undefined) info('Route Search to ' + p.n, 'Shortest path search is coming soon.', 'place');
          else info('Shortest Path to ' + p.n, p.r, 'place', 'See Map');
        };
      }

      var mb = document.getElementById('placeMap');
      if (p.m) { mb.style.display = ''; mb.innerHTML = '&#128506; ' + p.m; }
      else { mb.style.display = 'none'; }

      show('place');
    };
    placeBox.appendChild(b);
  });
</script>
</body>
</html>
)HTML";

    out.close();

    cout << "Opening Campus Connect in your browser..." << endl;

#if defined(_WIN32)
    system("start \"\" \"front_page.html\"");
#elif defined(__APPLE__)
    system("open front_page.html");
#else
    system("xdg-open front_page.html");
#endif

    return 0;
}
