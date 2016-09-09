﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CopterDown.Core;
using CopterDown.Messages;

namespace CopterDown.Game
{
    public class CopterDrawB : ABehavior
    {
        public CopterDrawB():base(ElementType.VIEW){}
        public override void OnMessage(Message msg)
        {
            
        }

        public override void Update(TimeSpan delta, TimeSpan absolute)
        {
            var frame = GameObject.FindAtt<int>(AT.AT_COM_FRAME);
            var transform = GameObject.GetTransform();
            var isHit = GameObject.FindAtt<bool>(AT.AT_COPTER_PARA_ISHIT);
            var leftDirection = GameObject.FindAtt<bool>(AT.AT_COPTER_LEFTDIRECTION);

            if (frame.Value++ % 10 < 5)
            {

                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter1.png", leftDirection.Value ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.0f, 0.0f,
                    5, !leftDirection.Value ? -1 : 1, 1);

            }
            else
            {
                Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/copter2.png", leftDirection.Value ? transform.LocalPos.X : transform.LocalPos.X + 111, transform.LocalPos.Y, 0, 0.5f, 0.0f,
                    5, !leftDirection.Value ? -1 : 1, 1);

            }

            if (isHit.Value) Helper.DrawImage(GameLoop._canvas, "pack://application:,,,/Images/explosion.png", transform.LocalPos.X, transform.LocalPos.Y, 0, 0.5f, 0.0f, 5, 1, 1);
        }
    }
}
