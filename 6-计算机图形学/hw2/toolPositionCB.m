function toolPositionCB(h, varargin)

%   Copyright © 2021, Renjie Chen @ USTC

set(h, 'Enable', 'off');

subplot(121);
set(h, 'Enable', 'on', 'UserData', [h.UserData, imline]);
