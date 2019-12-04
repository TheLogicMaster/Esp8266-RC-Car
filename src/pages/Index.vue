<template>
  <q-page class="flex flex-center">
    <q-dialog v-model="rgbMenu" persistent>
      <q-card>
        <q-card-section class="row items-center">
          <div class="text-h6">RGB Control</div>
          <q-space />
          <q-btn icon="close" flat round dense v-close-popup @click="updateState" />
        </q-card-section>

        <q-card-section>
          <q-tabs v-model="rgbType" no-caps class="shadow-2">
            <q-tab name="0" label="Color" />
            <q-tab name="1" label="Rainbow" />
          </q-tabs>
          <q-color
            v-if="rgbType === '0'"
            v-model="rgb"
            style="max-width: 250px"
          />
          <div v-else>
            <q-badge>Speed: {{ speeed }} (Default: 5)</q-badge>
            <q-slider v-model="speeed" :min="1" :max="10" :step="1"></q-slider>
          </div>
        </q-card-section>
        <q-card-section>
          <q-btn @click="sendRGB"> Submit </q-btn>
        </q-card-section>
      </q-card>
    </q-dialog>
    <q-dialog v-model="musicMenu" persistent>
      <q-card>
        <q-card-section class="row items-center">
          <div class="text-h6">Music Control</div>
          <q-space />
          <q-btn icon="close" flat round dense v-close-popup @click="updateState" />
        </q-card-section>

        <q-card-section>
          <q-select
            filled
            v-model="music"
            :options="musicOptions"
            label="Music:"
            emit-value
            map-options
          />
          <q-toggle
            label="Repeat"
            v-model="repeatMusic"
            color="blue"
          ></q-toggle>
          <div>
            <q-badge>Tempo: {{ tempo }} (Default: 144)</q-badge>
            <q-slider v-model="tempo" :min="0" :max="432"></q-slider>
          </div>
        </q-card-section>
        <q-card-section>
          <q-btn @click="sendMusic"> Submit </q-btn>
        </q-card-section>
      </q-card>
    </q-dialog>
    <q-btn label="Music" color="primary" @click="musicMenu = true" />
    <q-btn label="RGB" color="primary" @click="rgbMenu = true" />
    <div ref="left"></div>
    <div ref="right"></div>
  </q-page>
</template>

<script>
import nipplejs from "nipplejs";
import qs from "qs";
export default {
  name: "PageIndex",
  methods: {
    sendRGB() {
      let colors = this.rgb.slice(4, -1).split(",");
      this.$axios.post(
        "/rgb",
        qs.stringify({
          rgb_mode: this.rgbType,
          animation_speed: this.speeed,
          red: colors[0],
          green: colors[1],
          blue: colors[2]
        })
      );
      this.rgbMenu = false;
    },
    sendMusic() {
      this.$axios.post(
        "/music",
        qs.stringify({
          tempo: this.tempo,
          repeat: this.repeatMusic ? 1 : 0,
          song: this.music
        })
      );
      this.musicMenu = false;
    },
    sendUpdate() {
      if (
        (new Date().getTime() - this.lastMessage > 1000 &&
          (this.left !== 0 || this.right !== 0)) ||
        this.left !== this.leftPrev ||
        this.leftDir !== this.leftDirPrev ||
        this.right !== this.rightPrev ||
        this.rightDir !== this.rightDirPrev
      ) {
        this.leftPrev = this.left;
        this.leftDirPrev = this.leftDir;
        this.rightPrev = this.right;
        this.rightDirPrev = this.rightDir;
        this.$axios.post(
          "/data",
          qs.stringify({
            rightValue: this.right,
            rightDirection: this.rightDir,
            leftValue: this.left,
            leftDirection: this.leftDir
          })
        );
        this.lastMessage = new Date().getTime();
      }
    },
    sendNippleUpdate(evt, data, joystick) {
      if (evt["type"] === "end") {
        if (joystick === "left") {
          this.left = 0;
        } else {
          this.right = 0;
        }
        this.$axios.post(
          "/data",
          qs.stringify({
            rightValue: this.right,
            rightDirection: this.rightDir,
            leftValue: this.left,
            leftDirection: this.leftDir
          })
        );
      } else {
        if (joystick === "left") {
          this.left = data["distance"] > 10 ? data["distance"] : 0;
          this.leftDir = data["angle"]["degree"] === 90 ? 0 : 1;
        } else {
          this.right = data["distance"] > 10 ? data["distance"] : 0;
          this.rightDir = data["angle"]["degree"] === 90 ? 0 : 1;
        }
      }
    },
    updateState() {
      this.$axios.get("/get-state").then(result => {
        let vars = result.data.split(" ");
        this.rgbType = vars[0];
        this.speeed = parseInt(vars[1]);
        this.rgb = "rgb(" + vars[2] + "," + vars[3] + "," + vars[4] + ")";
        this.tempo = parseInt(vars[5]);
        this.repeatMusic = vars[6] === "1";
        this.music = vars[7];
        //console.log(result);
      });
    }
  },
  data() {
    return {
      lastMessage: new Date().getTime(),
      musicMenu: false,
      rgbMenu: false,
      repeatMusic: false,
      rgb: "rgb(0,0,0)",
      rgbType: "0",
      music: "-1",
      tempo: 144,
      speeed: 5,
      left: 0,
      leftDir: 0,
      right: 0,
      rightDir: 0,
      leftPrev: 0,
      leftDirPrev: 0,
      rightPrev: 0,
      rightDirPrev: 0,
      musicOptions: [
        {
          label: "No Music",
          value: "-1"
        },
        {
          label: "Pachabel's Canon",
          value: "0"
        },
        {
          label: "Mii Theme",
          value: "1"
        },
        {
          label: "Tetris Theme",
          value: "2"
        },
        {
          label: "Keyboard Cat",
          value: "3"
        },
        {
          label: "Take On Me",
          value: "4"
        },
        {
          label: "Pink Panther",
          value: "5"
        },
        {
          label: "Super Mario Bros",
          value: "6"
        },
        {
          label: "Dragon Ball Super",
          value: "7"
        },
        {
          label: "Spongebob Medley",
          value: "8"
        }
      ]
    };
  },
  created() {
    this.updateState();
    this.$nextTick(() => {
      //console.log(this.lastMessage)
      setInterval(() => this.sendUpdate(), 200);
      nipplejs
        .create({
          zone: this.$refs.left,
          mode: "static",
          position: { left: "20%", top: "50%" },
          color: "green",
          size: 200,
          lockY: true
        })
        .on(
          "move end",
          function(evt, data) {
            this.sendNippleUpdate(evt, data, "left");
          }.bind(this)
        );
      nipplejs
        .create({
          zone: this.$refs.right,
          mode: "static",
          position: { left: "80%", top: "50%" },
          color: "red",
          size: 200,
          lockY: true
        })
        .on(
          "move end",
          function(evt, data) {
            this.sendNippleUpdate(evt, data, "right");
          }.bind(this)
        );
    });
  }
};
</script>
