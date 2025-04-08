# Satellite Tracker mit STM32

Dieses Projekt entstand im Rahmen meines Studiums an der RWTH Aachen. Ziel war es, einen Roboter zu bauen, der autonom die Position eines Satellites (in unserem die International Space Station) verfolgen.

## Funktionen
- Firmware in C für STM32
- Projektplanung mit Gantt
- Berechnung von Azimut und Elevation, Koordinatenumrechnung

## Verwendete Technologien
- STM32CubeIDE
- Logic Analyzer, CoolTerm
- Python-API
- Git

## Herausforderungen
- Fehlersuche bei Signalverlusten
- Austausch defekter Komponenten
- Zeitmanagement & Teamkoordination

## Medien – Satellite Tracker
<div style="display:flex; flex-wrap:wrap; justify-content:space-between; gap:16px;">

  <div style="flex:1; min-width:300px; max-width:48%; border:1px solid #ccc; border-radius:8px; padding:16px; box-shadow:2px 2px 6px rgba(0,0,0,0.1);">
    <h3 style="margin-top:12px;">Projektplanung mit Gantt</h3>
    <p>Übersicht über die zeitliche Planung des Projekts, inklusive Meilensteine und Verantwortlichkeiten.</p>
    <img src="docs/media/gantt-chart.png" alt="Gantt Chart" width="100%" style="border-radius:4px;"/>
  </div>

  <div style="flex:1; min-width:300px; max-width:48%; border:1px solid #ccc; border-radius:8px; padding:16px; box-shadow:2px 2px 6px rgba(0,0,0,0.1);">
    <h3 style="margin-top:12px;">Systemdiagramm</h3>
    <p>Darstellung der Systemarchitektur: Datenfluss zwischen Python-API, STM32-Mikrocontroller und Robotikkomponenten.</p>
    <img src="docs/media/system-diagram.png" alt="System Diagram" width="100%" style="border-radius:4px;"/>
  </div>

  <div style="flex:1; min-width:300px; max-width:48%; border:1px solid #ccc; border-radius:8px; padding:16px; box-shadow:2px 2px 6px rgba(0,0,0,0.1);">
    <h3 style="margin-top:12px;">Aufbau des Trackers</h3>
    <p>Der Roboterarm besteht aus zwei Motoren – der obere Motor steuert die Elevation, der untere die Azimutrichtung. Positionsdaten der ISS werden lokal über eine Python-API abgerufen.</p>
    <img src="docs/media/tracker1.PNG" alt="Tracker Setup" width="100%" style="border-radius:4px;"/>
  </div>

  <div style="flex:1; min-width:300px; max-width:48%; border:1px solid #ccc; border-radius:8px; padding:16px; box-shadow:2px 2px 6px rgba(0,0,0,0.1);">
    <h3 style="margin-top:12px;">Demonstration & Verifikation</h3>
    <p>Im Testlauf folgte der Roboter der Position der ISS. Die Ausrichtung des Arms wurde visuell mit einer Sternbeobachtungs-App überprüft und bestätigt.</p>
    <img src="docs/media/tracker2.PNG" alt="Tracker Demo" width="100%" style="border-radius:4px;"/>
  </div>

</div>
