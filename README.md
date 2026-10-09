# ROUTEIQ — CMR Smart Bus Management

**Purpose:** A campus transport dashboard concept for safer, more transparent journeys.

## Current features

- Responsive dashboard with a CMR-inspired yellow, white and navy theme.
- Supabase email/password sign-in, account creation and sign-out.
- Supabase Auth session restoration on page load.
- Route, occupancy, fee, report and safety dashboard views.
- Searchable sample fee records and CSV route exports.
- Floating RouteIQ transport-help assistant.

## Authentication setup

The frontend uses the Supabase project configured in `index.html`. The browser uses the project's **publishable** key, which is designed for public frontend use. Never put a Supabase service-role/secret key in this repository.

In Supabase Dashboard → Authentication → URL Configuration, add the deployed GitHub Pages URL to the allowed Site URL / Redirect URLs. If email confirmation is enabled, new users must confirm their email before signing in.

A database migration creates a profile row automatically for new Auth users and adds owner-only read/update policies for `public.profiles`. New accounts receive the default `student` role. Do not trust a client-side role value to authorize administrative actions; implement and verify admin-only permissions server-side before adding privileged controls.

## Publish

1. Open repository **Settings → Pages**.
2. Select **Deploy from a branch**, branch `main`, folder `/(root)`.
3. Save and wait for deployment.
4. Visit https://paradoxloop18.github.io/ROUTE-IQ/

## Important limitations

Bus positions, passenger counts, route status, alerts and fee records are still **illustrative demo data**. The assistant is a local rule-based help preview, not a connected generative AI service. The site is not yet connected to GPS hardware, ESP32/GSM telemetry, live ETAs, real payments or operational transport records. Do not enter sensitive student or payment data.

The provided CMR logo image still needs to be added as a repository asset to display the exact supplied artwork; the current header uses a text-based CMR mark.

## Recommended next steps

1. Add the supplied CMR logo file under `assets/` and reference it in the header and sign-in screen.
2. Add real transport tables with row-level security and role-checked access.
3. Connect authenticated telemetry ingestion and map coordinates.
4. Add an AI provider through a Supabase Edge Function; keep provider API secrets server-side.
5. Test sign-up, email confirmation, sign-in, session persistence and sign-out on the deployed Pages URL.


## ESP32 GPS/GSM live bus tracking

The live tracking path now includes:

- `public.bus_locations` for GPS telemetry, with RLS enabled and authenticated-user read access.
- Supabase Realtime enabled for `bus_locations` so the dashboard can update map markers when new coordinates arrive.
- Supabase Edge Function `bus-location-ingest` (deployed) to accept device telemetry and insert it server-side.
- Example Arduino firmware: `firmware/routeiq_esp32_sim7600_gps.ino`.

### Required one-time configuration

1. In Supabase Dashboard → Edge Functions → Secrets, create `BUS_INGEST_SECRET` with a long random secret. Use the same value as `DEVICE_TOKEN` in the firmware. Do not commit a real device token.
2. The Edge Function uses Supabase's server-side environment variables `SUPABASE_URL` and `SUPABASE_SERVICE_ROLE_KEY`. Keep the service-role/secret key server-side only.
3. In `index.html`, replace `PASTE_RESTRICTED_GOOGLE_MAPS_API_KEY_HERE` with a Google Maps JavaScript API key restricted to the GitHub Pages HTTP referrer and the Maps JavaScript API. Billing may be required by Google.
4. Install Arduino libraries `TinyGSM` and `TinyGPSPlus`. The example targets an ESP32 + SIM7600 cellular modem and a separate UART GPS module; edit APN, pins, modem model and BUS_ID to match your hardware.
5. Flash the firmware, open Serial Monitor at 115200 baud, and verify it reports HTTP 200 after a fresh GPS fix. The bus should then appear on the signed-in ROUTEIQ map after the next telemetry update.

### Device and privacy safety

The Edge Function accepts requests without a Supabase user JWT because embedded devices authenticate with `x-device-token`; this is checked against the server-side `BUS_INGEST_SECRET`. Anyone with that device token could submit locations, so keep it private and rotate it if exposed. For production, use a separate token per bus, add rate limits and bus-ID authorization, and apply data-retention rules. The map is visible to authenticated users only. Never expose Supabase secret/service-role keys in firmware or browser code.

The supplied firmware is a hardware-adaptation reference, not a guarantee that every SIM7600 carrier board uses the same UART wiring, APN or modem settings. Test safely before deploying on a moving bus.
