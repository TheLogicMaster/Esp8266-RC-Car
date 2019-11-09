<template>
  <q-page class="flex flex-center">
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
    sendUpdate(evt, data, joystick) {
      if (evt["type"] === "end") {
        this.$axios.post(
          "/data/" + joystick,
          qs.stringify({
            value: 0,
            direction: 0
          })
        );
      } else {
        if (new Date().getTime() - this.lastMessage > 500) { // Todo: adjust me!
          //console.log(data)
          this.$axios.post(
            "/data/" + joystick,
            qs.stringify({
              value: data["distance"] > 10 ? data["distance"] : 0,
              direction: data["angle"]["degree"] === 90 ? 0 : 1
            })
          );
          this.lastMessage = new Date().getTime();
        }
      }
    }
  },
  data() {
    return {
      lastMessage: new Date().getTime()
    };
  },
  created() {
    this.$nextTick(() => {
      //console.log(this.lastMessage)
      //setInterval(() => this.sendUpdate())
      nipplejs.create({
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
            this.sendUpdate(evt, data, "left");
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
            this.sendUpdate(evt, data, "right");
          }.bind(this)
        );
    });
  }
};
</script>
