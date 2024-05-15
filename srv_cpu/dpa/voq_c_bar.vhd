-- voq_c_bar.vhd
-- Maryam Keyvani
-- Communication Networks Laboratory, Simon Fraser University
-- August 2001
-- This file is the VHDL source code for a DPA scheduler for a 4x4 ATM switch
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.std_logic_arith.ALL;
USE ieee.std_logic_unsigned.ALL;
LIBRARY lpm;
USE lpm.lpm_components.ALL;
USE work.voq_input_package.ALL;

ENTITY voq_c_bar IS
    GENERIC (NO_OF_PORTS: INTEGER := 4;
        PRIO_VEC_SIZE: INTEGER := 7 --Has to be [2(NO_OF_PORTS)-1]
    );

    PORT(
        arb_req: IN std_logic_vector(NO_OF_GRANTS_REQ DOWNTO 1);
        clk, reset : IN std_logic;
        grant : OUT std_logic_vector(NO_OF_GRANTS_REQ DOWNTO 1);
        P: OUT std_logic_vector(7 DOWNTO 1)
    );
END voq_c_bar;



ARCHITECTURE behaviour OF voq_c_bar IS
    COMPONENT Arbiter
        PORT(Req, North, West, Mask: IN std_logic;
        South, East, Grant: OUT std_logic);
    END COMPONENT;

--Cross Bar Signal Declarations
SIGNAL south_2_north : c_bar_signal_array;
SIGNAL east_2_west : c_bar_signal_array;
SIGNAL arb_mask : c_bar_signal_array;
SIGNAL arb_grant : c_bar_signal_array;
SIGNAL c_bar_P : STD_LOGIC_VECTOR (7 DOWNTO 1);
SIGNAL High : std_logic;
SIGNAL temp : INTEGER RANGE 1 to 2;

BEGIN
 grant(1) <= arb_grant(1)(1) or arb_grant(5)(1);
 grant(2) <= arb_grant(1)(2) or arb_grant(5)(2);
 grant(3) <= arb_grant(1)(3) or arb_grant(5)(3);
 grant(4) <= arb_grant(1)(4);
 grant(5) <= arb_grant(2)(1) or arb_grant(6)(1);
 grant(6) <= arb_grant(2)(2) or arb_grant(6)(2);
 grant(7) <= arb_grant(2)(3);
 grant(8) <= arb_grant(2)(4) or arb_grant(5)(4);

 grant(9) <= arb_grant(3)(1) or arb_grant(7)(1);
 grant(10) <= arb_grant(3)(2);
 grant(11) <= arb_grant(3)(3) or arb_grant(6)(3);
 grant(12) <= arb_grant(3)(4) or arb_grant(6)(4);

 grant(13) <= arb_grant(4)(1);
 grant(14) <= arb_grant(4)(2) or arb_grant(7)(2);
 grant(15) <= arb_grant(4)(3) or arb_grant(7)(3);
 grant(16) <= arb_grant(4)(4) or arb_grant(7)(4);


 P <= c_bar_P;
--This process rotates the priority vector
    Active_Win : process (clk, reset)
        BEGIN
        if reset = '0' then
            c_bar_P <= "0000000";
        elsif (clk = '1' and clk'event) then
            case c_bar_P is
            when "1111000" => c_bar_P <= "0111100";
            when "0111100" => c_bar_P <= "0011110";
            when "0011110" => c_bar_P <= "0001111";
            when "0001111" => c_bar_P <= "1111000";
            when others => c_bar_P <= "1111000";
            end case;
        end if;
    end process;
High <= '1';

--*************** Arbiter instantiation ************************************
--First Row
Arbiter_1_1: Arbiter
PORT MAP (Req => arb_req(1), North => High, West => High, Mask => c_bar_P(7),
 South => south_2_north(1)(1), East => east_2_west(1)(1) , Grant => arb_grant(1)(1));
Arbiter_1_2: Arbiter
PORT MAP (Req => arb_req(2), North => south_2_north(7)(2), West =>
east_2_west(1)(1), Mask => c_bar_P(6), South => south_2_north(1)(2), East =>
east_2_west(1)(2) , Grant => arb_grant(1)(2));
Arbiter_1_3: Arbiter
PORT MAP (Req => arb_req(3), North => south_2_north(7)(3), West =>
east_2_west(1)(2), Mask => c_bar_P(5), South => south_2_north(1)(3), East =>
east_2_west(1)(3) , Grant => arb_grant(1)(3));
Arbiter_1_4: Arbiter
PORT MAP (Req => arb_req(4), North => south_2_north(7)(4), West =>
east_2_west(1)(3), Mask => c_bar_P(4), South => south_2_north(1)(4), East =>
east_2_west(1)(4) , Grant => arb_grant(1)(4));

--Second Row
Arbiter_2_1: Arbiter
PORT MAP (Req => arb_req(5), North => south_2_north(1)(1), West =>
east_2_west(5)(4), Mask => c_bar_P(6), South => south_2_north(2)(1), East =>
east_2_west(2)(1) , Grant => arb_grant(2)(1));
Arbiter_2_2: Arbiter
PORT MAP (Req => arb_req(6), North => south_2_north(1)(2), West =>
east_2_west(2)(1), Mask => c_bar_P(5), South => south_2_north(2)(2), East =>
east_2_west(2)(2) , Grant => arb_grant(2)(2));
Arbiter_2_3: Arbiter
PORT MAP (Req => arb_req(7), North => south_2_north(1)(3), West =>
east_2_west(2)(2), Mask => c_bar_P(4), South => south_2_north(2)(3), East =>
east_2_west(2)(3) , Grant => arb_grant(2)(3));
Arbiter_2_4: Arbiter
PORT MAP (Req => arb_req(8), North => south_2_north(1)(4), West =>
east_2_west(2)(3), Mask => c_bar_P(3), South => south_2_north(2)(4), East =>
east_2_west(2)(4) , Grant => arb_grant(2)(4));

--Third Row
Arbiter_3_1: Arbiter
PORT MAP (Req => arb_req(9), North => south_2_north(2)(1), West =>
east_2_west(6)(4), Mask => c_bar_P(5), South => south_2_north(3)(1), East =>
east_2_west(3)(1) , Grant => arb_grant(3)(1));
Arbiter_3_2: Arbiter
PORT MAP (Req => arb_req(10), North => south_2_north(2)(2), West =>
east_2_west(3)(1), Mask => c_bar_P(4), South => south_2_north(3)(2), East =>
east_2_west(3)(2) , Grant => arb_grant(3)(2));
Arbiter_3_3: Arbiter
PORT MAP (Req => arb_req(11), North => south_2_north(2)(3), West =>
east_2_west(3)(2), Mask => c_bar_P(3), South => south_2_north(3)(3), East =>
east_2_west(3)(3) , Grant => arb_grant(3)(3));
Arbiter_3_4: Arbiter
PORT MAP (Req => arb_req(12), North => south_2_north(2)(4), West =>
east_2_west(3)(3), Mask => c_bar_P(2), South => south_2_north(3)(4), East =>
east_2_west(3)(4) , Grant => arb_grant(3)(4));

--Forth Row
Arbiter_4_1: Arbiter
PORT MAP (Req => arb_req(13), North => south_2_north(3)(1), West =>
east_2_west(7)(4), Mask => c_bar_P(4), South => south_2_north(4)(1), East =>
east_2_west(4)(1) , Grant => arb_grant(4)(1));
Arbiter_4_2: Arbiter
PORT MAP (Req => arb_req(14), North => south_2_north(3)(2), West =>
east_2_west(4)(1), Mask => c_bar_P(3), South => south_2_north(4)(2), East =>
east_2_west(4)(2) , Grant => arb_grant(4)(2));
Arbiter_4_3: Arbiter
PORT MAP (Re q => arb_req(15), North => south_2_north(3)(3), West =>
east_2_west(4)(2), Mask => c_bar_P(2), South => south_2_north(4)(3), East =>
east_2_west(4)(3) , Grant => arb_grant(4)(3));
Arbiter_4_4: Arbiter
PORT MAP (Req => arb_req(16), North => south_2_north(3)(4), West =>
east_2_west(4)(3), Mask => c_bar_P(1), South => south_2_north(4)(4), East =>
east_2_west(4)(4) , Grant => arb_grant(4)(4));

--Fifth Row
Arbiter_5_1: Arbiter
PORT MAP (Req => arb_req(1), North => south_2_north(4)(1), West =>
east_2_west(1)(4), Mask => c_bar_P(3), South => south_2_north(5)(1), East =>
east_2_west(5)(1) , Grant => arb_grant(5)(1));
Arbiter_5_2: Arbiter
PORT MAP (Req => arb_req(2), North => south_2_north(4)(2), West =>
east_2_west(5)(1), Mask => c_bar_P(2), South => south_2_north(5)(2), East =>
east_2_west(5)(2) , Grant => arb_grant(5)(2));
Arbiter_5_3: Arbiter
PORT MAP (Req => arb_req(3), North => south_2_north(4)(3), West =>
east_2_west(5)(2), Mask => c_bar_P(1), South => south_2_north(5)(3), East =>
east_2_west(5)(3) , Grant => arb_grant(5)(3));
Arbiter_5_4: Arbiter
PORT MAP (Req => arb_req(8), North => HIGH, West => HIGH, Mask =>
c_bar_P(7), South => south_2_north(5)(4), East => east_2_west(5)(4) , Grant =>
arb_grant(5)(4));

--Sixth Row
Arbiter_6_1: Arbiter
PORT MAP (Req => arb_req(5), North => south_2_north(5)(1), West =>
east_2_west(2)(4), Mask => c_bar_P(2), South => south_2_north(6)(1), East =>
east_2_west(6)(1) , Grant => arb_grant(6)(1));
Arbiter_6_2: Arbiter
PORT MAP (Req => arb_req(6), North => south_2_north(5)(2), West =>
east_2_west(6)(1), Mask => c_bar_P(1), South => south_2_north(6)(2), East =>
east_2_west(6)(2) , Grant => arb_grant(6)(2));
Arbiter_6_3: Arbiter
PORT MAP (Req => arb_req(11), North => HIGH, West => HIGH, Mask =>
c_bar_P(7), South => south_2_north(6)(3), East => east_2_west(6)(3) , Grant =>
arb_grant(6)(3));
Arbiter_6_4: Arbiter
PORT MAP (Req => arb_req(12), North => south_2_north(5)(4), West =>
east_2_west(6)(3), Mask => c_bar_P(6), South => south_2_north(6)(4), East =>
east_2_west(6)(4) , Grant => arb_grant(6)(4));

--Seventh Row
Arbiter_7_1: Arbiter
PORT MAP (Req => arb_req(9), North => south_2_north(6)(1), West =>
east_2_west(3)(4), Mask => c_bar_P(1), South => south_2_north(7)(1), East =>
east_2_west(7)(1) , Grant => arb_grant(7)(1));
Arbiter_7_2: Arbiter
PORT MAP (Req => arb_req(14), North => HIGH, West => HIGH, Mask =>
c_bar_P(7), South => south_2_north(7)(2), East => east_2_west(7)(2) , Grant =>
arb_grant(7)(2));
Arbiter_7_3: Arbiter
PORT MAP (Req => arb_req(15), North => south_2_north(6)(3), West =>
east_2_west(7)(2), Mask => c_bar_P(6), South => south_2_north(7)(3), East =>
east_2_west(7)(3) , Grant => arb_grant(7)(3));
Arbiter_7_4: Arbiter
PORT MAP (Req => arb_req(16), North => south_2_north(6)(4), West =>
east_2_west(7)(3), Mask => c_bar_P(5), South => south_2_north(7)(4), East =>
east_2_west(7)(4) , Grant => arb_grant(7)(4));
END behaviour;