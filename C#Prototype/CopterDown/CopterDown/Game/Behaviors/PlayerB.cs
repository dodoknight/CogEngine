﻿using System;
using System.Collections.Generic;
using System.Linq;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Enums;
using CopterDown.Game.Types;

namespace CopterDown.Game
{
    public class PlayerB : ABehavior
    {
        public PlayerB() : base(ElementType.MODEL, new State(GameActions.PARA_GROUNDED)){}

        public override void OnMessage(Message msg)
        {
            if (msg.Action == GameActions.PARA_GROUNDED)
            {
                GameObject.SceneRoot.FindAtt<int>(GameAttr.LIVES).Value--;
            }
        }

        

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var lives = GameObject.FindAtt<int>(GameAttr.LIVES);
            if (lives.Value == 0)
            {
                var root = GameObject.Parent;
                GameObject.Destroy();
                root.AddChild(new GameBuilder().CreateIntroScene());
                return;
            }

            var spawnInterval = GameObjectManager.Get.FindGameObjectBySubType(Subtypes.INFO).FindAtt<float>(GameAttr.SPAWNINTERVAL);
            var lastSpawn = GameObject.FindAtt<DateTime>(GameAttr.ACTUALSPAWN);

            if ((GameObject.Children == null || GameObjectManager.Get.GetGameObjectCountBySubType(Subtypes.COPTER) < 6)
                && ((DateTime.Now - lastSpawn.Value).TotalSeconds > 1.0 / spawnInterval.Value))
            {
                // spawn helicopter
                lastSpawn.Value = DateTime.Now;

                float posY = rnd.Next(0, 50);
                float posX = rnd.NextDouble() < 0.5 ? -111 : 640;

                bool leftdirection = posX == 640;

                GameObject copter = new GameBuilder().CreateCopter(new Transform(posX, posY), leftdirection);
                GameObject.SceneRoot.AddChild(copter);
            }

        }
    }
}
