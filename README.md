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
