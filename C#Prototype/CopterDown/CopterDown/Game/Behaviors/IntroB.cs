﻿using System;
using System.Linq;
using CopterDown.Core;
using CopterDown.Core.Entities;
using CopterDown.Core.Enums;
using CopterDown.Enums;
using CopterDown.Game.Types;

namespace CopterDown.Game
{
    public class IntroB : ABehavior
    {
        public IntroB() : base(ElementType.MODEL, new State()){}

        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var keys = GameObject.Root.FindAtt<UserAction>(Attr.USERACTION);

            if (keys.Value.KeyActions.Any(key => key.Ended))
            {
                var root = GameObject.Root;
                GameObject.Destroy();
                root.AddChild(new GameBuilder().CreateGameScene());
            }
        }
    }
}
