-- ROUTEIQ public demo access: read-only access to explicitly marked demo records.
-- This fixes the public dashboard's RLS errors without granting public write access.
-- Keep real student data private and keep all inserts/updates/deletes behind existing staff policies.

alter table if exists public.bus_routes enable row level security;
alter table if exists public.bus_stops enable row level security;
alter table if exists public.transport_students enable row level security;
alter table if exists public.bus_locations enable row level security;

drop policy if exists "Public can read demo routes" on public.bus_routes;
create policy "Public can read demo routes"
on public.bus_routes for select to anon, authenticated
using (is_demo = true);

drop policy if exists "Public can read demo stops" on public.bus_stops;
create policy "Public can read demo stops"
on public.bus_stops for select to anon, authenticated
using (is_demo = true);

drop policy if exists "Public can read demo student records" on public.transport_students;
create policy "Public can read demo student records"
on public.transport_students for select to anon, authenticated
using (is_demo = true);

-- The tracking map can read GPS points; no public write policy is added here.
drop policy if exists "Public can read bus locations" on public.bus_locations;
create policy "Public can read bus locations"
on public.bus_locations for select to anon, authenticated
using (true);
