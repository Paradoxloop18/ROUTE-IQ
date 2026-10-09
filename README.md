# ROUTEIQ — Smart Bus Management System

**Theme:** Sustainable Cities  
**Purpose:** A college transport dashboard concept for safer, more transparent and more sustainable campus journeys.

## Current prototype

The site is a responsive, static frontend in `index.html` featuring:
- Transport overview and sample fleet occupancy
- Illustrative fleet-map view
- Route and stop summaries
- Passenger insights
- Searchable demonstration transport-fee records
- Safety/service alerts and route-report CSV export
- System overview for the planned hardware and backend

## Important: demo, not live tracking

All bus locations, passenger counts, fee figures, route statuses and alerts currently displayed are **illustrative demo data**. This frontend is not connected to GPS hardware, ESP32, GSM/4G, passenger sensors, a backend API, or a database. Do not use the sample figures for operational decisions or enter real student/payment data.

## Planned architecture

- **Bus device:** ESP32 + GPS + GSM/4G + IR/ToF passenger sensors
- **Frontend:** HTML, CSS, JavaScript (Bootstrap may be added as the UI is developed)
- **Backend:** Python FastAPI
- **Database:** PostgreSQL
- **Planned integrations:** authenticated admin/student access, telemetry ingestion, ETA calculation, alerts, fee records and reports

## Publish with GitHub Pages

1. Open the repository **Settings → Pages**.
2. Under **Build and deployment**, choose **Deploy from a branch**.
3. Select branch `main` and folder `/(root)`, then click **Save**.
4. Wait for GitHub Pages to finish publishing. The expected project URL is:
   `https://paradoxloop18.github.io/ROUTE-IQ/`

The URL will only work after Pages is enabled and the deployment completes.

## Next implementation steps

1. Replace demo map and numbers with a real map provider and authenticated API.
2. Implement FastAPI endpoints and PostgreSQL schema for buses, routes, stops, students, fee status and passenger events.
3. Add device authentication, validation, timestamps and offline/stale-device handling.
4. Implement real ETA calculations and configurable safety/occupancy alerts.
5. Add privacy controls, role-based access, and secure handling of student and fee information.
