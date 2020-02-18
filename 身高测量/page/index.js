let app = getApp();
Page({
  onShareAppMessage() {
    return {
      title: '测量身高',
      path: 'page/index'
    }
  },
  data: {
    src:"../image/black.png",
    tempImagePath: "", // 拍照的临时图片地址
    ifRef:false,
    ctx: {},
    inputColorFocus:false,
    inputHeightFocus:false,
    inputNameFocus:false,
    hide:true,
    hide2:false,
    refColor:"颜色(黄色、红色、蓝色)",
    refHeight:"长度(cm)",
    measureName:"测量用户姓名"
  },
  onLoad(){
    this.setData({
      ctx: wx.createCameraContext(),
    })
  },
  cameraClick(){
    var that=this;
    let { ctx } = this.data;//等价于let ctx=this.data.ctx
    console.log("拍照");
    ctx.takePhoto({
      quality: "normal",
      success: (res) => {
        console.log("success!!");
        that.setData({
          tempImagePath: res.tempImagePath,
          src:res.tempImagePath,
          ifRef:true
        });
        console.log(that.data);
      },
      fail: (e) => {
        console.log(e);
      }
    });
  },
  tapColorInput(){
    this.setData({
      inputColorFocus:true,
      refColor:""
    })
  },
  tapHeightInput() {
    this.setData({
      inputHeightFocus: true,
      refHeight: ""
    })
  },
  tapNameInput() {
    this.setData({
      inputNameFocus: true,
      measureName: ""
    })
  },
  photoClick(){
   this.setData({
     hide:false,
     hide2:true
   });
  },
  returnClick(){
    this.setData({
      hide2: false,
      hide: true
    });
  },
  setColorValue:function(e){
    console.log("color");
    console.log(e);
    this.setData({
      refColor:e.detail.value,
      inputColorFocus:false
    })
    console.log(this.data);
  },
  setHeightValue: function (e) {
    console.log("height");
    console.log(e);
    this.setData({
      refHeight: e.detail.value,
      inputHeightFocus:false
    })
    console.log(this.data);
  },
  setNameValue: function (e) {
    console.log("Name");
    console.log(e);
    this.setData({
      measureName: e.detail.value,
      inputNameFocus: false
    })
    console.log(this.data);
  },
  confirm(){
    var that = this;
    that.setData({
      ifRef: false
    });
    setTimeout(function(){
      wx.showModal({
      title: '测量成功',
      content: "165cm",
      showCancel: false,
      success: function (res2) {
        wx.request({
          url: 'https://www.chenghuwei.cn//add.php',
          method: "GET",
          success: function (res) {
            console.log("ADD:");
            console.log(res.data);
          }
        })
      }
    })},6000);
 /*   wx.request({
      url: 'https://www.chenghuwei.cn//measure.php?a=' + this.data.tempImagePath + '&refColor=' + this.data.refColor + '&refHeight=' + this.data.refHeight +'&r=' + Math.random(),
          method: "GET",
          success: function (res) {
            that.setData({
              ifRef:false
            }),
            wx.showModal({
              title: '测量成功',
              content: res.data+"cm",
              showCancel: false,
              success: function (res2) {
                console.log(res.data);
              }
            })
          },
          fail:function(e){
            console.log("失败"+e);
          }
        })  */ 

  },
  cancel(){
    this.setData({
      ifRef:false
    })
  }
})
