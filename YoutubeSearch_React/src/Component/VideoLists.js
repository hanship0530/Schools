import React from 'react';
const VideoLists = ({video}) => {
  const videoItems = video.map((item, number) => {
    return (
      <li className="item" key={number.toString()}>
      <a href={`https://youtu.be/${item.id.videoId}`}>
      <h3>{item.snippet.title}</h3>
      <div className="image-wrapper">
      <img src={item.snippet.thumbnails.high.url} alt="description of img"/>
      </div>
      <div className="description">
      <small>By <span className="channel-title">{item.snippet.channelTitle}</span> on <time>{item.snippet.publishedAt}</time></small>
      <p>{item.snippet.description}</p>
      </div>
      </a>
      </li>
    );
  });

  return (
    <div id="layout-content" className="layout-content-wrapper">
    <div id="panel-div" className="panel-list">
    <ul id="results" className="item-list">
    { videoItems }
    </ul>
    </div>
    </div>
  )
}
export default VideoLists;
