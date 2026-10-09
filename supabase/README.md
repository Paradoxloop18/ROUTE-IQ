# ROUTEIQ Supabase data setup

The production Supabase project used by the GitHub Pages site is `gdlvhthtbcepzvblihtg`.

## Current tables

- `public.profiles` — Supabase Auth profile and role (`student`, `driver`, `admin`).
- `public.bus_locations` — GPS telemetry; the table is currently empty until a device posts coordinates.
- `public.bus_routes` — route code/name, bus plate and route incharge.
- `public.bus_stops` — stop order/code/name/type, schedule, coordinates and incharge.
- `public.transport_students` — roll number, name, year, department, campus, receipt label, boarding point and fee-status label.

## Current reference data

The live database currently contains **5 routes, 39 stops and 44 student transport records**. Every route, stop and student row has `is_demo = true`. These values were transcribed from the RouteIQ reference code provided in the conversation and are not verified official CMR transport records. The source supplied no fee amounts; only one record is marked Paid and 43 are marked Pending.

## Authentication and permissions

The website's sign-in/register dialog is embedded in `index.html`; there is no separate `auth.html` in this repository. It uses Supabase Auth email/password with the project's publishable browser key. Do not add a service-role key or admin password to frontend files.

Row Level Security is enabled on the new tables:
- Signed-in users can read route and stop records.
- Signed-in users can read explicitly marked demo student rows (and their own linked record, if one is linked); staff roles can read student records.
- Only a profile whose role is `admin` can write route, stop or student rows.
- A database trigger blocks a user from changing their own profile role through the browser. Role assignment must be performed by a trusted project administrator using a secure server-side/admin path.

No admin account or password was invented or created. Existing user accounts keep their current roles.

## Live integrations not yet verified

- Live bus positions need an ESP32/GPS device to post to the existing protected ingestion function and populate `bus_locations`.
- Fee-status values are reference/demo labels, not connected to a payment provider.
- Verify route plates, incharge names, times, coordinates, student records and payment status with the CMR transport office before operational use.
