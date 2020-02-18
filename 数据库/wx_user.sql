-- phpMyAdmin SQL Dump
-- version 4.8.0.1
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1
-- Generation Time: 2018-12-26 09:18:08
-- 服务器版本： 10.1.32-MariaDB
-- PHP Version: 7.2.5

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET AUTOCOMMIT = 0;
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `wx_user`
--

-- --------------------------------------------------------

--
-- 表的结构 `heightinfo`
--

CREATE TABLE `heightinfo` (
  `userNumber` varchar(20) NOT NULL,
  `measureDate` date NOT NULL,
  `measureName` varchar(20) NOT NULL,
  `height` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `heightinfo`
--

INSERT INTO `heightinfo` (`userNumber`, `measureDate`, `measureName`, `height`) VALUES
('bl151001215', '2018-12-20', '程小疯1', 182),
('bl151001215', '2018-12-21', '程小疯2', 181),
('bl151001215', '2018-12-22', '程小疯3', 189),
('bl151001215', '2018-12-23', '程小疯4', 184),
('bl151001215', '2018-12-24', '程小疯5', 181),
('bl151001215', '2018-12-26', '程小疯1', 182),
('bl151001215', '2018-12-27', '程小疯2', 183),
('C734194171', '2019-01-26', '程小疯1', 183),
('C734194171', '2018-03-26', '程小疯1', 183),
('C734194171', '2018-05-26', '程小疯1', 185);

-- --------------------------------------------------------

--
-- 表的结构 `userinfo`
--

CREATE TABLE `userinfo` (
  `userNumber` varchar(20) NOT NULL,
  `userName` varchar(20) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- 转存表中的数据 `userinfo`
--

INSERT INTO `userinfo` (`userNumber`, `userName`) VALUES
('bl151001215', 'xiao5'),
('C734194171', '程小疯');


--
-- Indexes for table `userinfo`
--
ALTER TABLE `userinfo`
  ADD PRIMARY KEY (`userNumber`);
--
-- 限制导出的表
--

--
-- 限制表 `heightinfo`
--
ALTER TABLE `heightinfo`
  ADD CONSTRAINT `heightinfo_ibfk_1` FOREIGN KEY (`userNumber`) REFERENCES `userinfo` (`userNumber`);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
