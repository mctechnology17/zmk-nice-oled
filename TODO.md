# 🐞 zmk-nice!beetle TODO

A living and ever expanding list of improvements and ideas to improve Beedly.

---

## ✅ Core Tasks

- [ ] Add Python-based workflow to automatically convert `.png` images into LVGL-compatible `.c` arrays  
  - [ ] Integrate `png_to_lvgl_c_indexed.py` with a `Makefile`
  - [ ] Output to a `generated/` folder with `.gitignore` support
  - [ ] Add requirements file for Python dependencies

---

## 🌠 Stretch Goals

- [ ] Replace current heart animation with an energy-style indicator  
  - [ ] Tie energy level to ZMK's shield battery percentage
  - [ ] Adjust animation speed or brightness based on charge level

- [ ] Add interactive Tamagotchi-like behavior to Beedly  
  - [ ] Track typing activity to simulate "feeding"
  - [ ] Idle timer to trigger "boredom" or "sleep"
  - [ ] Basic mood states (happy, hungry, tired, annoyed)
  - [ ] Optional: store "stats" in persistent memory or host-only flash

---

## 💡 Ideas (Backlog)

- [ ] Include `beedly.conf` for customization (thresholds, modes, etc.) once core behaviour is fleshed out
- [ ] Add more minigames and behaviours for Beedly

---