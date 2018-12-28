import React, { Component } from 'react';
import Search from './Component/Search'
import VideoLists from './Component/VideoLists'
import './App.css';

const API_KEY = 'key';
const maxResults = 6;
const url = 'https://www.googleapis.com/youtube/v3/search?'
class App extends Component {
  constructor(props) {
    super(props);
    this.state = {
      video: []
    }
    this.searchVideoWithKeyword('big short');
  }
  searchVideoWithKeyword(keyword) {
    fetch(url+'key='+API_KEY+'&part=snippet&type=video&maxResults='+maxResults+'&q='+keyword)
    .then(resp => resp.json())
    .then((resp) => {
      this.setState({video: resp.items});
    })
  }
  render() {
    return (
      <div className="App">
      <Search onSubmit={(keyword) => {this.searchVideoWithKeyword(keyword)}}/>
      <VideoLists video={this.state.video}/>
      </div>
    );
  }
}

export default App;
