import React, { Component } from 'react';
//import YoutubeApi from './YoutubeApi';
class Search extends Component {
  constructor(props) {
    super(props);
    this.state = {keyword:''}
    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({
      keyword: event.target.value
    });
  }

  handleSubmit(event) {
    event.preventDefault();
    this.props.onSubmit(this.state.keyword);
  }
  render() {
    return (
      <form id="search-form" method="get" action="" onSubmit={this.handleSubmit}>
      <div className="fieldcontainer">
      <input type="search" value={this.state.keyword} onChange={this.handleChange} id="query" name="keyword" className="search-field" placeholder="Search YouTube"/>
      <input type="submit" name="search-btn" className="search-btn" value="검색"/>
      </div>
      </form>
    );
  }
}
export default Search
